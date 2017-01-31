/*
* This file creates and runs the SDMS server on the specified port.
*/
var express = require('express');
var app = express();
var jsonfile = require('jsonfile');
var request = require('request');
var bunyan = require('bunyan');
var NodeCache = require('node-cache');
var cache = new NodeCache();
var bodyParser = require('body-parser');
var config = require('./config');
var spawn 	= require('child_process').spawn
var logger = bunyan.createLogger({name: "SDMS-Server", src: true});
var port = process.env.PORT || 8080;
var FIVE_MINUTES = 0.1 * 60000;
app.use(bodyParser.urlencoded({extended: false}))
app.use(bodyParser.json());
app.listen(port);

logger.info('Listening on port: ' + port);

// Every 5 minutes, read data from cache and determined if user 
// is stressed or not
setInterval(function() {
	// Read data from cache
	var cacheStats = cache.getStats();
	logger.info({stats: cacheStats}, 'Cache stats');

	// Write sensor data to file
	var fileName = './sensor_data.json';
	var obj = {name: 'JP'};
	jsonfile.writeFile(fileName, obj, function (err) {
		if (err) {
			logger.error({err: err}, 'Error writing to file');
		} else {
			logger.info({fileName: fileName}, 'Wrote to file');
		}
	});

	// Run machine learning algorithm on data
    var python 	= spawn('python', ['classify.py']);
	python.on('exit', function (code) {
		logger.info({result: code}, 'Classification');
		// empty cache
		cache.flushAll();
	});
}, FIVE_MINUTES);


//==================================SERVER API==================================//
app.get('/api', function(req, res) {
	logger.info({request_body: req.body}, 'Received request');
    res.json({message: 'Welcome to the SDMS API!'});   
});

app.get('/api/health', function(req, res) {
	logger.info({request_body: req.body}, 'Received request');
    res.json({message: 'API is healthy!'});   
});

app.get('/api/oauth/ihealth', function(req, res) {
	logger.info({request_query: req.query}, 'Received request');
	var code = req.query.code;
	if (!code) {
		res.json({message: 'User verification failed!'});
	} else {
		var url = config.ihealth.oauthUrl;
		var clientId = config.ihealth.clientId;
		var clientSecret = config.ihealth.clientSecret;
		var redirectUrl = config.ihealth.redirectUrl;
		var params = {
			client_id: clientId, 
			client_secret: clientSecret,
			grant_type: 'authorization_code',
			redirect_uri: redirectUrl,
			code: code
		};
		request({url: url, qs: params, json: true}, function(err, response, body) {
			if (err) { 
				logger.info({url: url, params: params}, 'Request failed!');
				res.json({message: 'User verification failed!'});
			} else if (res.statusCode == 200) {
				logger.info({body: body}, 'Response received');
				var output = {
					accessToken: body.AccessToken,
					userId: body.UserID,
					refreshToken: body.RefreshToken,
					accessTokenExpireTime: body.Expires,
					refreshTokenExpireTime: body.RefreshTokenExpires
				};
				res.json({message: output});
			} else {
				res.json({message: 'User verification failed!'});
			}
		});
	}
});

app.get('/api/oauth/spire', function(req, res) {
	logger.info({request_query: req.query}, 'Received request');
	var code = req.query.code;
});

app.post('/api/data', function(req, res, body) {
	logger.info({requestBody: req.body}, 'Received data');
	if (req.body) {
		addToCache(req.body)
	}
	res.json({res: 'success'});
});

// After receiving post request, store data in cache
function addToCache(data) {
	var gsr = data.GSR;
	if (gsr) {
		cache.get('GSR', function(err, value) {
			if (value) {
				var values = value.concat(gsr);
				cache.set("GSR", values);
			} else {
				cache.set("GSR", gsr);
			}
		});
	}
	var bvp = data.BVP;
	if (bvp) {
		cache.get('BVP', function(err, value) {
			if (value) {
				var values = value.concat(bvp);
				cache.set("BVP", values);
			} else {
				cache.set('BVP', bvp);
			}
		});
	}
	var ibi = data.IBI;
	if (ibi) {
		cache.get('IBI', function(err, value) {
			if (value) {
				var values = value.concat(ibi);
				cache.set("IBI", values);
			} else {
				cache.set('IBI', ibi);
			}
		});
	}
	logger.info('Caching data');
}