const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const app = express();
app.use(bodyParser.urlencoded({extended : false}));


var server = app.listen(8081, function () {
	var host = server.address().address;
	var port = server.address().port;
   
	console.log("Example app listening at http://%s:%s", host, port);
});


app.post('/', (req, res) => {
	console.log(req.body.Body);

	//Change the file directory as required
	//"req.body.Body" should not be changed
	fs.writeFile('C:\\Users\\HOME\\Desktop\\testFileNEW.txt', req.body.Body, 'utf8', function(err) {
  		if(err) {
  			return console.log(err);
  		}
    })
	res.send("Received");
});


app.post('/train', (req, res) => {
	console.log(req.body.Body);

	//Change the file directory as required
	//"req.body.Body" should not be changed
	//"req.body.Name" should not be changed
	var filename = req.body.Name + '.txt';
	fs.writeFile('C:\\Users\\HOME\\Desktop\\' + filename, req.body.Body, 'utf8', function(err) {
  		if(err) {
  			return console.log(err);
  		}
    })
	res.send("Received " + req.body.Name);
});

