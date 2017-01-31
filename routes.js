/* 
* This file contains all the routes for the SDMS API.
*
*/
var router = require('express').Router();
var request = require('request');
var bunyan = require('bunyan');
var NodeCache = require('node-cache');
var cache = new NodeCache();
var config = require('./config');
var logger = bunyan.createLogger({name: "SDMS-Routes", src: true});

router.get('/', function(req, res) {
	logger.info({request_body: req.body}, 'Received request');
    res.json({message: 'Welcome to the SDMS API!'});   
});

router.get('/health', function(req, res) {
	logger.info({request_body: req.body}, 'Received request');
    res.json({message: 'API is healthy!'});   
});

router.get('/oauth/ihealth', function(req, res) {
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

router.get('/oauth/spire', function(req, res) {
	logger.info({request_query: req.query}, 'Received request');
	var code = req.query.code;
});

router.post('/data', function(req, res, body) {
	logger.info({requestBody: req.body}, 'Received data');
	if (requestBody) {
		addToCache(requestBody)
	}
	res.json({res: 'success'});
});

// After receiving post request, store data in cache
function addToCache(data) {
	var gsr = data.GSR;
	if (gsr) {
		cache.get('GSR', function(err, value) {
			if (value) {
				value.concat(gsr);
			} else {
				cache.set("GSR", gsr);
			}
		});
	}
	var bvp = data.BVP;
	if (bvp) {
		cache.get('BVP', function(err, value) {
			if (value) {
				value.concat(bvp);
			} else {
				cache.set('BVP', bvp);
			}
		});
	}
	var ibi = data.IBI;
	if (ibi) {
		cache.get('IBI', function(err, value) {
			if (value) {
				value.concat(ibi);
			} else {
				cache.set('IBI', ibi);
			}
		});
	}
	logger.info('Caching data');
}

module.exports = router;