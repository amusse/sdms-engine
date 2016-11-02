/* 
* This file contains all the routes for the SDMS API.
*
*/
var router = require('express').Router();
var bunyan = require('bunyan');
var logger = bunyan.createLogger({name: "SDMS-Routes", src: true});

router.get('/', function(req, res) {
	logger.info({request_body: req.body}, 'Received request');
    res.json({message: 'Welcome to the SDMS API!'});   
});

router.get('/health', function(req, res) {
	logger.info({request_body: req.body}, 'Received request');
    res.json({message: 'API is healthy!'});   
});

module.exports = router;