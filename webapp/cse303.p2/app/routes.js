var qmanage = require('./qmanage.js');
var Session = require('./models/sessions.js');

// The main portion of our app is controlled by a state machine.  There is a
// collection of "sessions" (TODO: rename to surveys).  At most one can be in
// the active state at a time.
//
// An active session can be in the "new", "asking", "summarize", or "done"
// state.  Asking and Summarize move back and forth, with an integer
// associated with it, to ask and summarize answers to one question after the
// next.
//
// Moving the survey forward is done by the administrator
//
// Within the survey, we have an unauthenticated mechanism for preventing
// repeat answers to a question: the client-side javascript moves the local
// view from asking to summarize after the question is answered.  Since the
// survey is anonymous, this is pretty much all we can do.

module.exports = function(app, passport) {

    /**
     * public routes for survey-takers
     */

    // main route... this is for SAs who are participating in the survey.  it
    // just loads a shell for a single-page web app.
    app.get('/', function(req, res) {
        res.render('survey.ejs');
    });

    // CRUD route for getting the current content to display on the SA page
    app.get('/currq', function(req, res) {
        qmanage.getcurrent(req, res);
    });
    
    // CRUD route for adding an answer from the SA page
    app.post('/', function(req, res) {
        qmanage.answer(req, res);
    });

    /**
     * routes for login/logout/signup
     */
    // pages for login and signup
    app.get('/login', function(req, res) {
        res.render('login.ejs', { message: req.flash('loginMessage') }); 
    });
    app.get('/signup', function(req, res) {
        res.render('signup.ejs', { message: req.flash('signupMessage') });
    });
    // logout redirects to '/'
    app.get('/logout', function(req, res) {
        req.logout();
        res.redirect('/');
    });
    // CRUD routes for signup and login
    app.post('/signup', passport.authenticate('local-signup', {
        successRedirect : '/admin', 
        failureRedirect : '/signup', 
        failureFlash : true 
    }));
    app.post('/login', passport.authenticate('local-login', {
        successRedirect : '/admin',
        failureRedirect : '/login', 
        failureFlash : true 
    }));

    /**
     * Google authentication routes
     */
    // main route uses passport.js for OAuth authentication
    app.get('/auth/google', passport.authenticate('google', { scope : ['profile', 'email'] }));

    // the callback after google has authenticated the user
    app.get('/auth/google/callback',
            passport.authenticate('google', {
                successRedirect : '/admin',
                failureRedirect : '/googleadminfail'
            }));
    // if the google authentication fails, we redirect here.  It's not the
    // best way to do it, but it works.
    app.get('/googleadminfail', function(req, res) {
        res.render('login.ejs', { message: 'That Google account is not already registered, and new Google accounts are currently disabled.' });         
    });
    
    /**
     * Routes for admin to create survey sessions
     */
    
    // admin listing route... a single-page web app for creating survey
    // sessions and selecting one to run.
    app.get('/admin', isLoggedIn, function(req, res) {
        Session.find(function(err, result) {
            if (err) {
                console.log(err);
                res.end(ERROR);
                return;
            }
            res.render('session.ejs', {
                data: result
            });
        });
    });

    // CRUD route for making a new session
    app.post('/admin', isLoggedIn, function(req, res) {
        var s = new Session();
        s.name = req.body.name;
        s.currQ = 1;
        s.state = "new";
        s.active = 0;
        s.save(function(err, q) {
            if (err) {
                console.error(err);
                res.end("ERROR");
                return;
            }
            res.end(""+q._id);
        });
    });

    // CRUD route for activating a session
    app.put('/admin', isLoggedIn, function(req, res) {
        qmanage.activate(req, res);
    });

    /**
     * Routes for admin to conduct a survey
     */
    
    // admin session management page... another single-page web app
    app.get('/admin/:id', isLoggedIn, function(req, res) {
        res.render('sessionmanage.ejs');
    });

    // CRUD route for advancing a session
    app.put('/admin/:id', isLoggedIn, function(req, res) {
        qmanage.advance(req, res);
    });

    /**
     * Routes for printing summary information...
     */

    // list all summaries
    app.get('/summary', function(req, res) {
        Session.find(function(err, result) {
            if (err) {
                console.log(err);
                res.end(ERROR);
                return;
            }
            res.render('summaries.ejs', {
                data: result
            });
        });
    });
    
    // summary route for showing the "full survey summary" page... goes to a
    // single-page web app
    app.get('/summary/:id', function(req, res) {
        req.logout();
        Session.findOne({_id: req.params.id}, function(err, result) {
            if (err) {
                console.log(err);
                res.end(ERROR);
                return;
            }
            res.render('summary.ejs', {
                data: result._id,
                name : result.name
            });
        });
    });
    
    // CRUD route for getting raw data... works for sessions and for questions
    app.get('/summary/details/:id', function(req, res) {
        qmanage.summary(req, res);
    });
};

// middleware to make sure a user is logged in
function isLoggedIn(req, res, next) {
    // carry on if user is authenticated in session, else redirect home
    if (req.isAuthenticated())
        return next();
    res.redirect('/login');
}
