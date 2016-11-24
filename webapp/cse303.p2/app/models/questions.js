var mongoose = require('mongoose');

// define the schema for questions
var questionSchema = mongoose.Schema({
    qid : Number,     // the order of this question
    question: String, // question text
    ans1: String,     // first answer
    ans2: String,     // second answer
    ans3: String,     // third answer, or ""
    ans4: String,     // fourth answer, or ""
    ans5: String      // fifth answer, or ""
});

// create the model and expose it to our app
module.exports = mongoose.model('Question', questionSchema);
