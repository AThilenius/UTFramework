var express = require('express');
var router = express.Router();

/* GET home page. */
router.post('/', function(req, res, next) {
    console.log("Handling request: " + JSON.stringify(req.body));
    res.json( { all_is : "Good" } );    
});

module.exports = router;
