var mongoose = require('mongoose');

// define the schema for a session
var sessionSchema = mongoose.Schema({
    name   : String, // the name of the session, like 'SAM' or 'SA'
    currQ  : Number, // the current question being answered
    state  : String, // the state of the current question: new, done, asking, summarize
    active : Number  // 1 if active, 0 otherwise
});

// create the model and expose it to our app
module.exports = mongoose.model('Session', sessionSchema);
