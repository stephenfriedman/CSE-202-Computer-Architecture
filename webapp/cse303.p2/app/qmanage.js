var Question = require('./models/questions');
var Session = require('./models/sessions');
var Answer = require('./models/answers');

// memcached
var memcached = require('memcached');
var configmcd = require('../config/mcd.js');
var mcd = new memcached(configmcd.url);

module.exports = {

    // get the current question
    getcurrent : function(req, res) {
        Session.findOne({active:1}, function(err, session) {
            // no active session
            if (session === null) {
                res.end(JSON.stringify({error:"nosession"}));
                return;
            }
            Question.findOne({qid:session.currQ}, function(err, question) {
                var ans = {session:session, question:question};
                res.end(JSON.stringify(ans));
            });
        });
    },

    // enable the session, so we can start getting responses
    activate : function(req, res) {
        // make other sessions inactive, make this session active
        Session.update({active:1}, {active:0}, {multi:true}, function() {
            Session.update({_id:req.body.session}, {active:1}, {multi:false}, function() {
                res.end("OK");
            });
        });
    },

    // receive a POST answer from the survey
    answer : function(req, res) {
        var a = new Answer();
        a.session = req.body.session;
        a.question = req.body.question;
        a.answer = req.body.choice;
        a.save(function(err, a) {
            if (err) {
                console.error(err);
                res.end("ERROR");
                return;
            }
            res.end("OK");
        });
    },

    // advance the session
    advance : function(req, res) {
        // first get the session
        Session.findOne({active:1}, function(err, session) {
            // no active session
            if (session === null) {
                res.end(JSON.stringify({error:"nosession"}));
                return;
            }
            // figure out the new state and currQ
            var currQ = session.currQ;
            var state = session.state;
            var id    = session._id;
            // error if already done
            if (state === "done") {
                res.end("ERROR");
                return;
            }
            // if new, move to asking
            if (state === "new") {
                Session.update({_id:id}, {state:"asking"}, {multi:false}, function() {
                    res.end("OK");
                    return;
                });
            }
            // if asking, move to summarizing
            if (state === "asking") {
                Session.update({_id:id}, {state:"summarize"}, {multi:false}, function() {
                    res.end("OK");
                    return;
                });
            }
            // if summarizing, check if there's another question, and either
            // close out or advance
            if (state === "summarize") {
                Question.findOne({qid:(currQ+1)}, function(err, question) {
                    if (question === null) {
                        Session.update({_id:id}, {state:"done"}, {multi:false}, function() {
                            res.end("OK");
                            return;
                        });
                    }
                    else {
                        var d = {state:"asking", currQ:(currQ+1)};
                        Session.update({_id:id}, d, {multi:false}, function() {
                            res.end("OK");
                            return;
                        });

                    }
                });
            }
        });
    },

    // get summary data, either for a question or for a whole session
    summary : function(req, res) {
        // is there a session id that matches?
        Session.findOne({_id : req.params.id}, function(err, session) {
            var query = {};
            var qquery = {};
            // no: query for the question
            if (session == null) {
                // note: we don't have joins in mongo, so we need to get the
                // answers and the question separately
                query.question = req.params.id;
                qquery._id = req.params.id; 
                Answer.find(query, function(err, answers){
                    if (err) {
                        console.error(err);
                        res.end("ERROR");
                    }
                    Question.find(qquery, function(err, questions){
                        if (err) {
                            console.error(err);
                            res.end("ERROR");
                        }
                        var ans = {
                            q : questions,
                            a : answers
                        }
                        res.end(JSON.stringify(ans))
                    });
                });
            }
            // yes: query for the session
            else {
                query.session = req.params.id;
                // first, let's see if it's in the cache
                mcd.get(""+query.session, function(err, result) {
                    // error... get from database, update cache
                    if (err) {
                        console.error(err);
                        doUpdateCache(query.session, query, qquery, res);
                    }
                    // good result... just return it (we trust mcd)
                    if (result) {
                        console.log("mcd hit");
                        res.end(result);
                    }
                    // no result... get from database, update cache
                    else {
                        doUpdateCache(query.session, query, qquery, res);
                    }
                });
            }
        });
    }
};

// get all the results for a sessionid, build JSON, return it, and also
// update MCD.
function doUpdateCache(sessionid, query, qquery, res) {
    Answer.find(query, function(err, answers){
        if (err) {
            console.error(err);
            res.end("ERROR");
        }
        Question.find(qquery, function(err, questions){
            if (err) {
                console.error(err);
                res.end("ERROR");
            }
            var ans = {
                q : questions,
                a : answers
            }
            res.end(JSON.stringify(ans))
            var oneday = 86400; // 24 hours
            mcd.set(""+sessionid, JSON.stringify(ans), oneday, function(err, result) {
                if (err)
                    console.error(err);
                console.log("mcd response: " + result);
            });
        });
    });
}
