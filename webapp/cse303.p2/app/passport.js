var LocalStrategy  = require('passport-local').Strategy;
var User           = require('../app/models/user');
var GoogleStrategy = require('passport-google-oauth').OAuth2Strategy;
var User           = require('./models/user');
var configAuth     = require('../config/auth');

module.exports = function(passport) {

    // serialize the user for the session
    passport.serializeUser(function(user, done) {
        done(null, user.id);
    });
    
    // deserialize the user
    passport.deserializeUser(function(id, done) {
        User.findById(id, function(err, user) {
            done(err, user);
        });
    });

    // log in with local account
    passport.use('local-login', new LocalStrategy({
        usernameField : 'email',
        passwordField : 'password',
        passReqToCallback : true
    },
    function(req, email, password, done) {
        // is email address in database?
        User.findOne({ 'local.email' :  email }, function(err, user) {
            // unspecified error?
            if (err) return done(err);
            // no such user?
            if (!user)
                return done(null, false, req.flash('loginMessage', 'No user found.'));
            // bad password?
            if (!user.validPassword(password))
                return done(null, false, req.flash('loginMessage', 'Oops! Wrong password.'));
            // all good
            return done(null, user);
        });
    }));
    
    // sign up for local account
    passport.use('local-signup', new LocalStrategy({
        usernameField : 'email',
        passwordField : 'password',
        passReqToCallback : true
    },
    function(req, email, password, done) {
        // not sure why we need this, but we do
        process.nextTick(function() {
            // Check if user exists
            User.findOne({ 'local.email' :  email }, function(err, user) {
                // unspecified error?
                if (err)
                    return done(err);
                // user exists?
                if (user)
                    return done(null, false, req.flash('signupMessage',
                                                       'That email is already taken.'));
                // sign-ups disabled?
                if (!configAuth.allowNewUser)
                    return done(null, false, req.flash('signupMessage',
                                                       'New local accounts are currently disabled.'));
                // create and save user
                var newUser            = new User();
                newUser.local.email    = email;
                newUser.local.password = newUser.generateHash(password);
                newUser.save(function(err) {
                    if (err)
                        throw err;
                    return done(null, newUser);
                });
            });
        });
    }));

    // log in with google account
    passport.use(new GoogleStrategy({
        clientID        : configAuth.googleAuth.clientID,
        clientSecret    : configAuth.googleAuth.clientSecret,
        callbackURL     : configAuth.googleAuth.callbackURL,
    },
    function(token, refreshToken, profile, done) {
        // not sure why we need this, but we do... something about waiting
        // for a reply from Google
        process.nextTick(function() {
            // Check if user exists
            User.findOne({ 'google.id' : profile.id }, function(err, user) {
                // unspecified error?
                if (err)
                    return done(err);
                // user exists?  If so, log user in
                if (user) 
                    return done(null, user);
                // sign-ups disabled?
                // NB: flash messages are tricky from an OAuth callback, so
                // we do a double-redirect instead of a flash
                if (!configAuth.allowNewUser)
                    return done(null, false);
                // create and save user
                var newUser          = new User();
                newUser.google.id    = profile.id;
                newUser.google.token = token;
                newUser.google.name  = profile.displayName;
                newUser.google.email = profile.emails[0].value; // pull the first email
                newUser.save(function(err) {
                    if (err)
                        throw err;
                    return done(null, newUser);
                });
            });
        });
    }));    
};
