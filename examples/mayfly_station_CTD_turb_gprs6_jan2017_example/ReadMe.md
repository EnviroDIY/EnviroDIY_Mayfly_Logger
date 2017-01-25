Mayfly station CTD turb gprs6 example
===

* Filename: mayfly_station\_CTD\_turb\_gprs6\_jan2017\_example.ino
* Written By:  Shannon Hicks <shicks@stroudcenter.org>
* Documentation By: Anthony Aufdenkampe <aaufdenkampe@limno.com>
* Creation Date: 2016
* Development Environment: Arduino IDE 1.6.x
* Hardware Platform: Stroud Center, EnviroDIY Mayfly Arduino Datalogger
* Radio Module:  GPRSbee rev.6 cell wireless
* Sensors:
  * Decagon Devices CTD-10 sensor
  * Campbell Scientific OBS-3+ sensor

This sketch is an example sketch for a solar-powered cell-radio wireless stream sensing station based on those recently deployed at Delaware’s First State National Historic Park (FSNHP), as described in our [EnviroDIY Mayfly logger stations deployed in PA, DE and MN!](http://envirodiy.org/envirodiy-mayfly-logger-stations-deployed-in-pa-de-and-mn/) blog post. For this deployment, the data are posted to a data system at [http://swrcsensors.dreamhosters.com](http://swrcsensors.dreamhosters.com), which can only be accessed by Stroud Center staff. 

**WARNING:** This sketch will not work in it's entirity as it is currently written, because the data are posted to a dummy RESTful endpoint URL 
(http://somewebsite.com/somescript.php?) using the syntax that is expected at by our system at http://swrcsensors.dreamhosters.com. However, this sketch could be modified to post sensor measurements from any sensor to any receiving data system that has been configured to accept them.