/* 
* This file contains all the routes for the SDMS API.
*
*/
var router = require('express').Router();
var request = require('request');
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

router.get('/oauth', function(req, res) {
	logger.info({request_query: req.query}, 'Received request');
	var code = req.query.code;
	if (!code) {
		res.json({message: 'User verification failed!'});
	} else {
		var url = 'https://api.ihealthlabs.com:8443/OpenApiV2/OAuthv2/userauthorization/';
		var clientId = '2919ba70fea043ddb1c85fdbfdaa20ae';
		var clientSecret = 'd096af91d05c4994bc1230c13b9bd87e';
		var redirectUrl = 'http://96418cba.ngrok.io/api/oauth/';
		var params = { 
			client_id: clientId, 
			client_secret: clientSecret,
			grant_type: 'authorization_code',
			redirect_uri: redirectUrl,
			code: code
		};
		request({url: url, qs: params}, function(err, response, body) {
			if (err) { 
				logger.info({url: url, params: params}, 'Request failed!');
				res.json({message: 'User verification failed!'});
			} else if (res.statusCode == 200) {
				logger.info({body: body}, 'Response received');
				var accessToken = body.AccessToken;
				var userId = body.UserID;
				var refreshToken = body.RefreshToken;
				var accessTokenExpireTime = body.Expires;
				var refreshTokenExpireTime = body.RefreshTokenExpires;
				res.json({message: 'User verification succeeded!'});
			} else {
				res.json({message: 'User verification failed!'});
			}
		});
	}
});

module.exports = router;