/**
 *  This is a utility script for dropping the questions table, and then
 *  re-populating it with new questions.
 */

// connect to the database
var mongoose = require('mongoose');
var configDB = require('./config/database.js');
mongoose.connect(configDB.url);

// load the schema for entries in the 'questions' table
var Question = require('./app/models/questions');

// here are the questions we'll load into the database.  Field names don't
// quite match with the schema, but we'll be OK.
var questionlist = [
    {
        i:1,
        q:"On average, how many hours a day do you dedicate to your sport?",
        a:"Less than 1",
        b:"1-2",
        c:"2-3",
        d:"3-4",
        e:"More than 4"
    },
    {
        i:2,
        q:"On average, how many hours a day do you dedicate to academics outside of the classroom?",
        a:"Less than 1",
        b:"1-2",
        c:"2-3",
        d:"3-4",
        e:"More than 4"
    },
    {
        i:3,
        q:"On average, how many hours of continuous sleep do you get each day?",
        a:"Less than 4",
        b:"4-7",
        c:"More than 7",
        d:"",
        e:""
    },
    {
        i:4,
        q:"Have you been late or absent from class?",
        a:"Yes",
        b:"No",
        c:"",
        d:"",
        e:""
    },
    {
        i:5,
        q:"Have you been late or absent from practice?",
        a:"Yes",
        b:"No",
        c:"",
        d:"",
        e:""
    },
    {
        i:6,
        q:"How would you rate your academic performance this semster?",
        a:"Better than I expected",
        b:"About what I expected",
        c:"Worse than I expected",
        d:"",
        e:""
    },
    {
        i:7,
        q:"How would you rate your athletic performance this semster?",
        a:"Better than I expected",
        b:"About what I expected",
        c:"Worse than I expected",
        d:"",
        e:""
    },
    {
        i:8,
        q:"How would you rate your overall Lehigh experience this semster?",
        a:"Better than I expected",
        b:"About what I expected",
        c:"Worse than I expected",
        d:"",
        e:""
    },
    {
        i:9,
        q:"Have you introduced yourself to your professors?",
        a:"All of them",
        b:"Some of them",
        c:"None of them",
        d:"",
        e:""
    },
    {
        i:10,
        q:"Can you name your professors?",
        a:"All of them",
        b:"Some of them",
        c:"None of them",
        d:"",
        e:""
    },
    {
        i:11,
        q:"How often have you used on-campus academic resources (office hours, tutoring, library, etc)?",
        a:"Often",
        b:"A few times",
        c:"Never",
        d:"",
        e:""
    },
    {
        i:12,
        q:"Do you look at your phone while at practice?",
        a:"Yes",
        b:"No",
        c:"",
        d:"",
        e:""
    },
    {
        i:13,
        q:"Do you look at your phone while studying?",
        a:"Yes",
        b:"No",
        c:"",
        d:"",
        e:""
    },
    {
        i:14,
        q:"Do you look at your phone during class?",
        a:"Yes",
        b:"No",
        c:"",
        d:"",
        e:""
    },
];

// drop all data, and if the drop succeeds, run the insertion callback
Question.remove({}, function(err) {
    // count successful inserts, and exit the process once the last insertion
    // finishes (or any insertion fails)
    var received = 0;
    for (var i = 0; i < questionlist.length; ++i) {
        var q = new Question();
        q.qid = questionlist[i].i;
        q.question = questionlist[i].q;
        q.ans1= questionlist[i].a;
        q.ans2= questionlist[i].b;
        q.ans3= questionlist[i].c;
        q.ans4= questionlist[i].d;
        q.ans5= questionlist[i].e;
        q.save(function(err, q) {
            if (err) {
                console.error(err);
                process.exit();
            }
            received++;
            if (received == questionlist.length)
                process.exit();
        });
    }
});
