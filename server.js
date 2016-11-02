/*
* This file creates and runs the SDMS server on the specified port.
*
*/
var express = require('express');
var app = express();
var router = require('./routes.js');
var bunyan = require('bunyan');
var bodyParser = require('body-parser');

var logger = bunyan.createLogger({name: "SDMS-Server", src: true});
var port = process.env.PORT || 8080;

app.use(bodyParser.urlencoded({extended: false}))
app.use(bodyParser.json());
app.use('/api', router);
app.listen(port);

logger.info('Listening on port: ' + port);