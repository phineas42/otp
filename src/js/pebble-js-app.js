var OTP = {};
//this routing is for dev only, not production:
OTP.ua2hex = function(ua) {
    var h = '';
    for (var i = 0; i < ua.length; i++) {
      h += (ua[i]<16?'0':'')+ua[i].toString(16);
    }
    return h;
};

OTP.base32decode = function(s) {
  var bufsize = Math.floor((s.length*5+7)/8);
  //var buf = new Uint8Array(bufsize);
  var buf = new Array(bufsize);
  var bufindex, offset, value, overflow;
  var c,i;
  //Uint8Array is zeroed automatically
  for (i=0;i<bufsize;i++) {
    buf[i]=0;
  }
  for (i=s.length-1,bufindex=bufsize-1,offset=0; i>=0; i--) {
    c=s.charCodeAt(i);
    if (c>=0x41 && c<=0x5A) { //where 0x41 is 'A' and 0x5A is 'Z'
      value = c-0x41; // where 0x41 is 'A'
    } else if (c>=0x61 && c<=0x7A) { //where 0x61 is 'a' and 0x7A is 'z'
      value = c-0x61; // where 0x61 is 'a'
    } else {
      value = c-0x32+26; // where 0x32 is '2'
    }
    console.log("Processing: " + value);
    buf[bufindex] |= value<<offset;
    buf[bufindex] &= 0xff;
    offset+=5; //output offset is 5 bits farther to the left
    if (offset>7) {
      //the new offset spills into the next byte
      overflow=value>>(13-offset); // 13 is 8+5
      bufindex--;
      buf[bufindex] |= overflow;
      offset-=8;
    }
  }
  return buf;
};
OTP.isvalidbase32 = function(s) {
  var alphabet = /^[A-Za-z2-7]+$/;  
   if(s.match(alphabet)) {
     return true;
   } else {
     return false;
   }
};

Pebble.addEventListener('ready',
  function(e) {
    console.log('JavaScript app ready and running!');
  }
);
Pebble.addEventListener('showConfiguration', function(e) {
  //var o = Pebble.openURL('data:text/html;base64,PGh0bWw+PGhlYWQ+PG1ldGEgbmFtZT0idmlld3BvcnQiIGNvbnRlbnQ9IndpZHRoPTMyMCwgaW5pdGlhbC1zY2FsZT0xIj48c3R5bGU+bGFiZWwse2Rpc3BsYXk6YmxvY2t9bGFiZWx7Y2xlYXI6Ym90aDtoZWlnaHQ6NTBweH1pbnB1dHtmbG9hdDpyaWdodH08L3N0eWxlPjwvaGVhZD48Ym9keT48aDE+UGViYmxlSlMgYmxvYXRlZCBpbmxpbmUgSFRNTDwvaDE+PGZvcm0gaWQ9ImYiIG9uc3VibWl0PSJyZXR1cm4gcyh0aGlzKSI+PGxhYmVsPkZvbzxpbnB1dCB0eXBlPSJjaGVja2JveCIgbmFtZT0iZm9vIj48L2xhYmVsPjxsYWJlbD5CYXI8aW5wdXQgdHlwZT0iY2hlY2tib3giIG5hbWU9ImJhciI+PC9sYWJlbD48bGFiZWw+U2F2ZTxpbnB1dCB0eXBlPSJzdWJtaXQiPjwvbGFiZWw+PC9mb3JtPjxzY3JpcHQ+ZnVuY3Rpb24gcyhlKXtmb3Iobz17fSxpPTA7aTxlLmxlbmd0aDtpKyspKGo9ZVtpXS5uYW1lKSYmKG9bal09ZVtpXS5jaGVja2VkPzE6MCk7cmV0dXJuIHdpbmRvdy5sb2NhdGlvbi5ocmVmPSJwZWJibGVqczovL2Nsb3NlIyIrSlNPTi5zdHJpbmdpZnkobyksITF9PC9zY3JpcHQ+PC9ib2R5PjwvaHRtbD4K');
  //var o = Pebble.openURL('data:text/html;base64,PGh0bWw+PGhlYWQ+PG1ldGEgbmFtZT0idmlld3BvcnQiIGNvbnRlbnQ9IndpZHRoPTMyMCwgaW5pdGlhbC1zY2FsZT0xIj48c3R5bGU+bGFiZWwse2Rpc3BsYXk6YmxvY2t9bGFiZWx7Y2xlYXI6Ym90aDtoZWlnaHQ6NTBweH1pbnB1dHtmbG9hdDpyaWdodH08L3N0eWxlPjwvaGVhZD48Ym9keT48aDE+T1RQPC9oMT48Zm9ybSBpZD0iZiIgb25zdWJtaXQ9InJldHVybiBzKHRoaXMpIj48bGFiZWw+S2V5IChiYXNlMzIpPGlucHV0IHR5cGU9InRleHQiIG5hbWU9ImtleTAiPjwvbGFiZWw+PGxhYmVsPlNhdmU8aW5wdXQgdHlwZT0ic3VibWl0Ij48L2xhYmVsPjwvZm9ybT48c2NyaXB0PmZ1bmN0aW9uIHMoZSl7Zm9yKG89e30saT0wO2k8ZS5sZW5ndGg7aSsrKShqPWVbaV0ubmFtZSkmJihvW2pdPWVbaV0uY2hlY2tlZD8xOjApO3JldHVybiB3aW5kb3cubG9jYXRpb24uaHJlZj0icGViYmxlanM6Ly9jbG9zZSMiK0pTT04uc3RyaW5naWZ5KG8pLCExfTwvc2NyaXB0PjwvYm9keT48L2h0bWw+Cg==');
  //var o = Pebble.openURL('data:text/html;base64,PGh0bWw+PGhlYWQ+PG1ldGEgbmFtZT0idmlld3BvcnQiIGNvbnRlbnQ9IndpZHRoPTMyMCwgaW5pdGlhbC1zY2FsZT0xIj48c3R5bGU+bGFiZWwse2Rpc3BsYXk6YmxvY2t9bGFiZWx7Y2xlYXI6Ym90aDtoZWlnaHQ6NTBweH1pbnB1dHtmbG9hdDpyaWdodH08L3N0eWxlPjwvaGVhZD48Ym9keT48aDE+T1RQPC9oMT48Zm9ybSBpZD0iZiIgb25zdWJtaXQ9InJldHVybiBzKHRoaXMpIj48bGFiZWw+S2V5IChiYXNlMzIpPGlucHV0IHR5cGU9InRleHQiIG5hbWU9ImtleTAiLz48L2xhYmVsPjxpbnB1dCB0eXBlPSJmaWxlIiBhY2NlcHQ9ImltYWdlLyoiIGNhcHR1cmU9ImNhbWVyYSI+Q2FtZXJhPC9pbnB1dD48bGFiZWw+U2F2ZTxpbnB1dCB0eXBlPSJzdWJtaXQiLz48L2xhYmVsPjwvZm9ybT48c2NyaXB0PmZ1bmN0aW9uIHMoZSl7Zm9yKG89e30saT0wO2k8ZS5sZW5ndGg7aSsrKShqPWVbaV0ubmFtZSkmJihvW2pdPWVbaV0uY2hlY2tlZD8xOjApO3JldHVybiB3aW5kb3cubG9jYXRpb24uaHJlZj0icGViYmxlanM6Ly9jbG9zZSMiK0pTT04uc3RyaW5naWZ5KG8pLCExfTwvc2NyaXB0PjwvYm9keT48L2h0bWw+Cg==');
  //var o = Pebble.openURL('https://rawgit.com/LazarSoft/jsqrcode/master/src/test.html');
  Pebble.openURL('http://phineas42.github.io/otp/config/');
});
       /*                 
Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  //Pebble.openURL('data:text/html;base64,' +
  //               'PGh0bWw+DQo8aGVhZD4NCjx0aXRsZT5Db25maWd1cmUhPC90aXRsZT4NCjwvaGVhZD4NCjxib2R5PjxoMT5Db25maWd1cmUhPC9oMT48L2JvZHk+DQo8L2h0bWw+' +
  //               '<!--.html');
  //Pebble.openURL('data:text/html;charset=US-ASCII,' +
  //                '<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1"/><title>Configure!</title></head><body><header><h1>Configure!</h1></header><input type="checkbox" id="showdate"/><label for="showdate">Display Date:</label></body></html><!--.html');
  
});
*/
Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log('Configuration window returned: ', JSON.stringify(configuration));
    //console.log('Configuration from form: ' + )
    //verify input
    var config = {};
    var sendconfig = false;
    if (OTP.isvalidbase32(configuration.otpkey0) && configuration.otpkey0.length==16) {
      config.MK_OTPKEY0 = OTP.base32decode(configuration.otpkey0);
      //window.localStorage.setItem('OTPKEY0', e.response);
      sendconfig=true;
    }
    if (! configuration.otplabel0) {
      configuration.otplabel0 = "otplabel0"
    }
    if (configuration.otplabel0.length<=10) {
      config.MK_OTPLABEL0 = configuration.otplabel0;
      sendconfig=true;
    }
    if (sendconfig) {
      console.log('Dump: ' + JSON.stringify(config));
      Pebble.sendAppMessage(
        config,
        function(e) {
          console.log('Successfully delivered message with transactionId=' + e.data.transactionId);
        },
        function(e, msg) {
          console.log('Unable to deliver message with transactionId=' + e.data.transactionId + ' Error is: ' + msg);
        }
      );
    }
  }
);