// Called when JS is ready
Pebble.addEventListener("ready",
							function(e) {
								setTimeout(function() {
									Pebble.sendAppMessage({ 0: 2, 1: "test", 2: "5 points"});									
								}, 1000);
							});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
							function(e) {
								console.log("Received Status: " + e.payload[0]);
								console.log("Received Message: " + e.payload[1]);
							});