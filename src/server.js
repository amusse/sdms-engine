/*
* Ahmed Musse
*
*
* This file creates and runs the SDMS server on the specified port.
*/
var fs = require('fs');
var express = require('express');
var app = express();
var jsonfile = require('jsonfile');
var request = require('request');
var bunyan = require('bunyan');
var apn = require('apn');
var NodeCache = require('node-cache');
var cache = new NodeCache();
var bodyParser = require('body-parser');
var config = require('./config');
var spawn 	= require('child_process').spawn
var logger = bunyan.createLogger({name: "SDMS-Server", src: true});
var port = process.env.PORT || 8080;
var FOUR_MINUTES = 4.3 * 60000;
var phase = 0;

app.use(bodyParser.json({limit: '50mb'}));
app.use(bodyParser.urlencoded({limit: '50mb', extended: true, parameterLimit: 50000}));
app.listen(port);

// iOS Push Notification Setup
// Set up apn with the APNs Auth Key
var apnProvider = new apn.Provider({  
     token: {
        key: './apn_key.p8', 
        keyId: 'PV6EWLLQBR', 
        teamId: 'NZ8SAPA6G5', 
    },
    production: false // Set to true if sending a notification to a production iOS app
});

// Enter the device token from the Xcode console
var deviceToken = '75A27B9936C0114EEB2B292C0EB07E4C5760702A1D1AAE57C874CDC1A73CCF68';

// Prepare a new notification
var notification = new apn.Notification();

// Specify your iOS app's Bundle ID (accessible within the project editor)
notification.topic = 'com.RAN.dStress';

// Set expiration to 1 hour from now (in case device is offline)
notification.expiry = Math.floor(Date.now() / 1000) + 3600;

logger.info('Listening on port: ' + port);

function parseData() {
	// Read data from cache
	var cacheStats = cache.getStats();
	logger.info({stats: cacheStats}, 'Cache stats');

	cache.keys(function(err, mykeys) {
		if(!err) {
			var length = mykeys.length;
			var date = new Date();
			var time = date.getTime()/1000;

			var path = "./src/tests/test07"
			
			phase++;
			// Write data to files to be read
			for (var i = 0; i < length; i++) {
				if (mykeys[i] == "BVP") {
					var bvpValues = cache.get(mykeys[i])
					for (var value in bvpValues) {
						var data = bvpValues[value][0] + "," + bvpValues[value][1] + "\n"
						fs.appendFileSync(path + '/BVP/BVP_data_phase' + phase + '.csv', data);
					}
				}
				if (mykeys[i] == "IBI") {
					var ibiValues = cache.get(mykeys[i])
					for (var value in ibiValues) {
						var data = ibiValues[value][0] + "," + ibiValues[value][1] + "\n"
						fs.appendFileSync(path + '/IBI/IBI_data_phase' + phase + '.csv', data);
					}
				}
				if (mykeys[i] == "GSR") {
					var gsrValues = cache.get(mykeys[i])
					for (var value in gsrValues) {
						var data = gsrValues[value][0] + " " + gsrValues[value][1] + "\n"
						fs.appendFileSync(path + '/GSR/GSR_data_phase' + phase + '.txt', data);
					}
				}
				if (mykeys[i] == "TEMP") {
					var tempValues = cache.get(mykeys[i])
					for (var value in tempValues) {
						var data = tempValues[value][0] + " " + tempValues[value][1] + "\n"
						fs.appendFileSync(path + '/TEMP/TEMP_data_phase' + phase + '.txt', data);
					}
				}
				if (mykeys[i] == "BO") {
					var boValues = cache.get(mykeys[i])
					for (var value in boValues) {
						var data = boValues[value][0] + " " + boValues[value][1] + " " + boValues[value][2] + " " + boValues[value][3] + "\n"
						fs.appendFileSync(path + '/BO/BO_data_phase' + phase + '.txt', data);
					}
				}
				if (mykeys[i] == "BP") {
					var bpValues = cache.get(mykeys[i])
					for (var value in bpValues) {
						var data = bpValues[value][0] + " " + bpValues[value][1] + " " + bpValues[value][2] +  " " + bpValues[value][3] + " " + bpValues[value][4] + "\n"
						fs.appendFileSync(path + '/BP/BP_data_phase' + phase + '.txt', data);
					}
				}
			}

			// Extract Ledalab features
			// var matlab = spawn('matlab', ['-nodesktop', '-nosplash','-r', "process_gsr"])
		 //    matlab.stderr.on('data', function(d) {
		 //    	logger.info("Error Received")
		 //    	logger.info(d)
		 //    });
		 //    matlab.stdout.on('data', function(d, other) {
		 //    	logger.info("Data Received")
		 //    	console.log(other)
		 //    });
		 //    matlab.on('close', function() {
		 //    	logger.info("Finished extracting Ledalab features")

		 	// Extract BVP features
			// var matlab = spawn('matlab', ['-nodesktop', '-nosplash','-r', "process_bvp"])
		 //    matlab.stderr.on('data', function(d) {
		 //    	logger.info("Error Received")
		 //    	logger.info(d)
		 //    });
		 //    matlab.stdout.on('data', function(d, other) {
		 //    	logger.info("Data Received")
		 //    	console.log(other)
		 //    });
		 //    matlab.on('close', function() {
		 //    	logger.info("Finished extracting BVP features")


		 //    	// Extract other features and classify if stressed or not
			//     var python 	= spawn('python', ['classify.py', time]);
			// 	python.on('exit', function (code) {
			// 		logger.info({result: code}, 'Classification');
			// 		// empty cache
			// 		cache.flushAll();
			// 	});	

		 //    });



	  	}
	});
	cache.flushAll();
}

function classifyData() {
	// Read data from cache
	var cacheStats = cache.getStats();
	logger.info({stats: cacheStats}, 'Cache stats');

	cache.keys(function(err, mykeys) {
		if(!err) {
			var length = mykeys.length;
			var date = new Date();
			var time = parseInt(date.getTime()/1000);

			var path = "./data"
			
			// Write data to files to be read
			for (var i = 0; i < length; i++) {
				if (mykeys[i] == "BVP") {
					var bvpValues = cache.get(mykeys[i])
					for (var value in bvpValues) {
						var data = bvpValues[value][0] + "," + bvpValues[value][1] + "\n"
						fs.appendFileSync(path + '/BVP_data_' + time + '.csv', data);
					}
				}
				if (mykeys[i] == "IBI") {
					var ibiValues = cache.get(mykeys[i])
					for (var value in ibiValues) {
						var data = ibiValues[value][0] + "," + ibiValues[value][1] + "\n"
						fs.appendFileSync(path + '/IBI_data_' + time + '.csv', data);
					}
				}
				if (mykeys[i] == "GSR") {
					var gsrValues = cache.get(mykeys[i])
					for (var value in gsrValues) {
						var data = gsrValues[value][0] + " " + gsrValues[value][1] + "\n"
						fs.appendFileSync(path + '/GSR_data_' + time + '.txt', data);
					}
				}
				if (mykeys[i] == "TEMP") {
					var tempValues = cache.get(mykeys[i])
					for (var value in tempValues) {
						var data = tempValues[value][0] + " " + tempValues[value][1] + "\n"
						fs.appendFileSync(path + '/TEMP_data_' + time + '.txt', data);
					}
				}
				if (mykeys[i] == "BO") {
					var boValues = cache.get(mykeys[i])
					for (var value in boValues) {
						var data = boValues[value][0] + " " + boValues[value][1] + " " + boValues[value][2] + " " + boValues[value][3] + "\n"
						fs.appendFileSync(path + '/BO_data_' + time + '.txt', data);
					}
				}
				if (mykeys[i] == "BP") {
					var bpValues = cache.get(mykeys[i])
					for (var value in bpValues) {
						var data = bpValues[value][0] + " " + bpValues[value][1] + " " + bpValues[value][2] +  " " + bpValues[value][3] + " " + bpValues[value][4] + "\n"
						fs.appendFileSync(path + '/BP_data_' + time + '.txt', data);
					}
				}
			}
			logger.info("Extracting matlab features")

			var matlab = spawn('matlab', ['-nodesktop', '-nosplash','-r', 'process_matlab_files_rt']);
			matlab.stderr.on('data', function(d) {
				logger.info("Error Received")
				logger.info(d)
			});
			matlab.on('close', function() {
				logger.info("Finished extracting matlab features");
				logger.info("Classifying Data");
				var python 	= spawn('python', ['./src/classify.py']);
				python.on('exit', function (code) {
					logger.info({result: code}, 'Classification');
					// empty cache
					if (code == 1) {
						// Set app badge indicator
						notification.badge = 3;

						// Play ping.aiff sound when the notification is received
						notification.sound = 'ping.aiff';

						notification.alert = 'It seems like you are stressed. Want to listen to some classical music?';
						// Display the following message (the actual notification text, supports emoji)

						// Send any extra payload data with the notification which will be accessible to your app in didReceiveRemoteNotification
						notification.payload = {id: 123};

						// Actually send the notification
						apnProvider.send(notification, deviceToken).then(function(result) {  
						    // Check the result for any failed devices
						    logger.indo({res: result}, 'Notification Sent');
						});
					}
					cache.flushAll();
				});	
			});
	  	}
	});
	cache.flushAll();
}

// Set app badge indicator
notification.badge = 3;

// Play ping.aiff sound when the notification is received
notification.sound = 'ping.aiff';

notification.alert = 'It seems like you are stressed. Want to listen to some classical music?';
// Display the following message (the actual notification text, supports emoji)

// Send any extra payload data with the notification which will be accessible to your app in didReceiveRemoteNotification
notification.payload = {id: 123};

// Actually send the notification
apnProvider.send(notification, deviceToken).then(function(result) {  
// Check the result for any failed devices
logger.indo({res: result}, 'Notification Sent');
});
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
		var accessToken = req.query.AccessToken;
		if (!accessToken) {
			res.json({message: 'User verification failed!'});
		} else {
			logger.info({body: req.query}, 'Received accessToken');
			res.json({message: req.query});
		}
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
		addToCache(req.body);
		parseData();
	}
	res.json({res: 'success'});
});

app.post('/api/classify', function(req, res, body) {
	logger.info({requestBody: req.body}, 'Received data');
	if (req.body) {
		addToCache(req.body);
		classifyData();
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
	var bo = data.BO;
	if (bo) {
		cache.get('BO', function(err, value) {
			if (value) {
				var values = value.concat(bo);
				cache.set("BO", values);
			} else {
				cache.set('BO', bo);
			}
		});
	}
	var temp = data.TEMP;
	if (temp) {
		cache.get('TEMP', function(err, value) {
			if (value) {
				var values = value.concat(temp);
				cache.set("TEMP", values);
			} else {
				cache.set('TEMP', temp);
			}
		});
	}
	var bp = data.BP;
	if (bp) {
		cache.get('BP', function(err, value) {
			if (value) {
				var values = value.concat(bp);
				cache.set("BP", values);
			} else {
				cache.set('BP', bp);
			}
		});
	}
	logger.info('Caching data');
}