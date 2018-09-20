

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
	fs.writeFile("C:\\Users\\HOME\\Desktop\\testFileNEW.txt", req.body.Body, 'utf8', function(err) {
  	if(err) {
  		return console.log(err);
  	}
  	console.log("The file was saved");
  })


	res.send("Received");
});

