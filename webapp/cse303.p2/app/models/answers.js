var mongoose = require('mongoose');

// define the schema for an answer to a question
var answerSchema = mongoose.Schema({
    session  : String, // the ID of the session
    question : String, // the ID of the question
    answer   : Number  // the selected answer (1-5)
});

// create the model and expose it to our app
module.exports = mongoose.model('Answer', answerSchema);
