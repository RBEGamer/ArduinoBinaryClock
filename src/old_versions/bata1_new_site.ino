<!DOCTYPE html>
<html lang="en">
<head>
    <title>CLOCK ADMIN PAGE</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta name='apple-mobile-web-app-capable' content='yes' />
    <meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />
    <link rel="stylesheet" href="chromoselector.css" />
		<script src="jstimezonedetect.js" type="text/javascript"></script>
    <script src="jquery-1.10.2.min.js" type="text/javascript"></script>
    <script src="chromoselector.min.js" type="text/javascript"></script>
    <script type="text/javascript">
    $(document).ready(function () {
    $('#color_nth').chromoselector({
        width: 130,
        //panelAlpha: true,
        resizable: false,
        color2str: function (color) {
            return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 
        }
    })
	
	
	$('#color_nthz').chromoselector({
        width: 130,
       
        resizable: false,
        color2str: function (color) {
            return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 
        }
    })
	
	
	$('#custom_on_h').chromoselector({width: 130,resizable: false,color2str: function (color) {return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 }})
	
	
	$('#custom_on_m').chromoselector({width: 130,resizable: false,color2str: function (color) {return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 }})
	
		$('#custom_off_s').chromoselector({width: 130,resizable: false,color2str: function (color) {return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 }})
	
		$('#custom_off_h').chromoselector({width: 130,resizable: false,color2str: function (color) {return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 }})
		$('#custom_off_m').chromoselector({width: 130,resizable: false,color2str: function (color) {return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 }})
		$('#custom_on_s').chromoselector({width: 130,resizable: false,color2str: function (color) {return color.getRgbaString().replace(/[rgba()]/g , '').substring(0,color.getRgbaString().replace(/[rgba()]/g , '').length-2);	 }})
	
	
});

    </script>
    <style type="text/css">
    body {
        margin: 2em;
		 
		background-image:url(http://clock.rbnetworks.de/bg.jpg);background-repeat:repeat;
    } 
    </style>
</head>
<body>
<center>
<h1> CLOCK ADMIN PAGE </h1>

   <hr>
    <p>&nbsp;</p>
    
    <h2> CURRENT TIME : 00:00:00</h2>
      <p>&nbsp;</p>
 
    <table width="60%" border="1">
    
    
      <tr><td scope="col" colspan="4">&nbsp;  </td>  </tr>
    <tr><td scope="col" colspan="4"><h2> LED SETTINGS </h2></td>   </tr>
   <form id='ledsettings' name='ledsettings' action=''>
   <tr>
    <td><label for="ledmode">LED EFFECT</label> </td>
    <td scope="col"><input type="radio" name="ledmode" id="ledmode" value="0" >OFF</td>  
    <td scope="col"><input type="radio" name="ledmode" id="ledmode"  value="1" checked>CUSTOM 1</td>
     <td scope="col"><input type="radio" name="ledmode" id="ledmode"  value="2" checked>RAINBOW</td>
  </tr>
   
   <tr>
    <td scope="col"> <label for="custom_on_h">CUSTOM MODE <br> LED ON</label></td>
    <td>
    HOUR : <br>
    MINUTE : <br>
    SECOND : 
    </td>
    
    <td scope="col" colspan="3">
      <input id="custom_on_h" name="custom_on_h" type="text" value="0,0,0" placeholder="[R,G,B]" /><br>
     <input id="custom_on_m" name="custom_on_m" type="text" value="0,0,0" placeholder="[R,G,B]" /><br>
      <input id="custom_on_s" name="custom_on_s" type="text" value="0,0,0" placeholder="[R,G,B]" />
     </td>
   
  </tr>
  
  
  <tr>
    <td scope="col"> <label for="custom_off_h">CUSTOM MODE <br> LED OFF</label></td>
    <td>
    HOUR : <br>
    MINUTE : <br>
    SECOND : 
    </td>
    
    <td scope="col" colspan="3">
      <input id="custom_off_h" name="custom_off_h" type="text" value="0,0,0" placeholder="[R,G,B]" /><br>
     <input id="custom_off_m" name="custom_off_m" type="text" value="0,0,0" placeholder="[R,G,B]" /><br>
      <input id="custom_off_s" name="custom_off_s" type="text" value="0,0,0" placeholder="[R,G,B]" />
     </td>
   
  </tr>
   
        <tr><td scope="col" colspan="4"><center><input type='submit' value='SAVE LED SETTINGS' /></center></td>   </tr>
     
     
     </form>
     <form id='nightmodesettings' name='nightmodesettings' action=''>
    <tr><td scope="col" colspan="4">&nbsp;  </td>  </tr>
    <tr><td scope="col" colspan="4"><h2> NIGHTMODE SETTINGS </h2></td>   </tr>
   
    
    
    
  <tr>
    <td scope="col"> <label for="color_nthz">NIGHMODE LED-OFF COLOR</label></td>
    <td scope="col" colspan="3"> <input id="color_nthz" name="my-color_nthz" type="text" value="0,0,0" placeholder="[R,G,B]" /></td>
   
  </tr>
  
  <tr>
    <td scope="col"><label for="color_nth">NIGHMODE LED-ON COLOR</label></td>
    <td scope="col" colspan="3"><input id="color_nth" name="color_nth" type="text" value="0,0,0" placeholder="[R,G,B]" /></td>
   
  </tr>
  
  
  <tr>
    <td><label for="nth_en">NIGHTMODE STATE</label> </td>
    <td scope="col"><input type="radio" name="nth_en" id="nth_en"  value="1" checked>ENABLE</td>
    <td scope="col"><input type="radio" name="nth_en" id="nth_en" value="0" >DISABLE</td>  
    <td scope="col" colspan="0">&nbsp;  </td>
  </tr>
  
 
    <tr><td scope="col" colspan="4"><center><input type='submit' value='SAVE NIGHTMODE SETTINGS' /></center></td>   </tr>
  </form>
  <form id='alarmsettings' name='alarmsettings' action=''>
    <tr><td scope="col" colspan="4">&nbsp;  </td>  </tr>
    <tr><td scope="col" colspan="4"><h2> ALARM SETTINGS </h2></td>   </tr>
   
  <tr>
    <td><label for="alarm_h">ALARM TIME [hh:mm]</label></td>
    <td colspan="3"><input name="alarm_h" type="number" id="alarm_h" max="23" min="0" step="1" value="0" size="5"> : <input name="alarm_m" type="number" id="alarm_m" max="59" min="0" step="1" value="0" size="5"></td>
    
  </tr>
  <tr>
    <td><label for="alarm_en">ALARM STATE</label> </td>
    <td ><input type="radio" name="alarm_en" id="alarm_en" value="1" checked>ENABLE</td>
    <td ><input type="radio" name="alarm_en" id="alarm_en" value="0" >DISABLE</td>
    <td>&nbsp;</td>  
  </tr>
 
    <tr><td scope="col" colspan="4"><center><input type='submit' value='SAVE ALARM SETTINGS' /></center></td>   </tr>
  <tr><td scope="col" colspan="4">&nbsp;  </td>  </tr>
  
   </form>
   <form id='generalsettings' name='generalsettings' action=''>
    <tr><td scope="col" colspan="4"><h2> GENERAL SETTINGS </h2></td>   </tr>
    
      <tr>
    <td><label for="button_en">BUTTON STATE</label> </td>
    <td ><input type="radio" name="button_en" id="button_en" value="1" checked>ENABLE</td>
    <td ><input type="radio" name="button_en" id="button_en" value="0" >DISABLE</td>
    <td>&nbsp;</td>  
  </tr>
  
  <tr>
    <td><label for="timezone">TIMEZONE [hh] </label></td>
    <td colspan="3"><input name="timezone" type="number" id="timezone" max="23" min="0" step="1" value="0" ></td>  
  </tr>
  
  
  
    
     <tr><td scope="col" colspan="4"><center><input type='submit' value='SAVE GENERAL SETTINGS' /></center></td>   </tr>
  
  </form>
  <form id='networksettings' name='networksettings' action=''>
  
  <tr><td scope="col" colspan="4">&nbsp;  </td>  </tr>
    <tr><td scope="col" colspan="4"><h2> NETWORK SETTINGS [ADVANCED] </h2></td>   </tr>
   
  <tr>
    <td><label for="dhcp_en">NDHCP_STATE</label> </td>
    <td ><input type="radio" name="dhcp_en" id="dhcp_en" value="1" checked>ENABLE</td>
    <td ><input type="radio" name="dhcp_en" id="dhcp_en" value="0" >DISABLE</td>
    <td>&nbsp;</td>  
  </tr>
  
  
    <tr>
    <td><label for="ntp_ip">NTP-SERVER IP</label></td>
    <td colspan="3"><input type="text" name="ntp_ip" id="ntp_ip" value="132.163.4.103" placeholder="xxx.xxx.xxx.xxx"></td>  
  </tr>
  
  
  <tr>
    <td><label for="ip">IP</label></td>
    <td colspan="3"><input type="text" name="ip" id="ip" value="192.168.178.58" placeholder="xxx.xxx.xxx.xxx"></td>  
  </tr>
  
  <tr>
    <td><label for="gateway">GATEWAY</label></td>
    <td colspan="3"><input type="text" name="gateway" id="gateway" value="192.168.178.1" placeholder="xxx.xxx.xxx.xxx"></td>  
  </tr>
  
  <tr>
    <td><label for="subnet">SUBNET</label></td>
    <td colspan="3"><input type="text" name="subnet" id="subnet" value="255.255.255.0" placeholder="xxx.xxx.xxx.xxx"></td>  
  </tr>
  
 
 
 
   <tr><td scope="col" colspan="4"><center><input type='submit' value='SAVE NETWORK SETTINGS' /></center></td>   </tr>
  <tr><td scope="col" colspan="4">&nbsp;  </td>  </tr>
  
  </form>
    <tr><td scope="col" colspan="4"><h2> CLOCK INFORMATION </h2></td>   </tr>
   
   <tr>
    <td>SOFTWARE VERSION</td>
    <td colspan="3">1023</td>  
  </tr>
  
  <tr>
    <td>MAC</td>
    <td colspan="3">DE:AD:BE:EF:FE:ED</td>  
  </tr>
  
</table>
    
       
        
      
        
        
        
        
</form>
</center>
</body>
</html>
