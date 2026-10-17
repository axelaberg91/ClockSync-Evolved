#pragma once
// Web interface and status display for the clocksync fork by axelaberg91.
#include <pgmspace.h>

static const char WEB_UI[] PROGMEM = R"CLOCKSYNC(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="theme-color" content="#f4f5f6">
<title>clocksync evolved</title>
<style>
:root{color-scheme:light;--bg:#f4f5f6;--paper:#fff;--ink:#222b30;--muted:#68747b;--line:#dce1e5;--green:#17694b;--soft:#eaf5eb;--amber:#8a5a10;--red:#a33832}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--ink);font:15px/1.5 system-ui,-apple-system,Segoe UI,sans-serif}main{max-width:1020px;margin:auto;padding:32px 24px 24px}header{display:flex;align-items:center;justify-content:space-between;margin-bottom:40px}.brand{display:flex;align-items:center;gap:11px;font-size:21px;font-weight:720;letter-spacing:-.8px}.mark{display:grid;place-items:center;width:34px;height:34px;border-radius:10px;background:var(--ink);color:#fff}.device{font-size:12px;color:var(--muted);border:1px solid var(--line);border-radius:20px;padding:6px 12px}.eyebrow{text-transform:uppercase;font-size:11px;letter-spacing:1.8px;font-weight:700;color:var(--muted)}h1{font-size:29px;font-weight:650;letter-spacing:-1px;line-height:1.2;margin:9px 0 23px}.layout{display:grid;grid-template-columns:1.4fr 1fr;gap:18px}.card{background:var(--paper);border:1px solid var(--line);border-radius:10px;padding:25px}.hero{display:flex;flex-direction:column;justify-content:space-between;min-height:306px}.status{display:inline-flex;align-items:center;gap:8px;align-self:flex-start;border-radius:20px;background:#f0f2ef;color:var(--muted);padding:6px 11px;font-size:12px;font-weight:650}.dot{width:7px;height:7px;border-radius:50%;background:currentColor}.status.transmitting{background:var(--soft);color:var(--green)}.status.waiting,.status.aligning{background:#fff4df;color:var(--amber)}.status.stale,.status.error{background:#ffede9;color:var(--red)}.time{font-size:clamp(42px,7vw,66px);font-weight:500;letter-spacing:-3px;line-height:1.1;font-variant-numeric:tabular-nums;margin:25px 0 3px}.date{color:var(--muted);font-size:13px}.message{font-size:13px;color:var(--muted);max-width:440px;margin:24px 0 0;min-height:39px}.actions{display:flex;gap:10px;margin-top:22px;flex-wrap:wrap}button{min-height:43px;border:1px solid var(--line);background:white;color:var(--ink);border-radius:10px;padding:10px 15px;font:inherit;font-size:13px;font-weight:650;cursor:pointer}button.primary{background:var(--ink);border-color:var(--ink);color:white}button:hover{filter:brightness(.95)}button:disabled{opacity:.5;cursor:wait}button:focus-visible,input:focus-visible,select:focus-visible,summary:focus-visible{outline:3px solid #8dbba8;outline-offset:3px}.section-label{font-size:14px;font-weight:650;margin:0 0 21px}.row{display:flex;align-items:baseline;justify-content:space-between;gap:14px;padding:12px 0;border-bottom:1px solid #edf0eb;font-size:13px}.row:last-child{border:0}.row span{color:var(--muted)}.row strong{font-weight:550;text-align:right}.freshness{grid-column:1/-1;display:flex;align-items:center;gap:28px}.freshness .copy{flex:1}.freshness p{margin:4px 0 0;font-size:13px;color:var(--muted)}.remaining{font-size:24px;letter-spacing:-.6px;font-variant-numeric:tabular-nums;white-space:nowrap}.meter{height:5px;background:#e9ede7;border-radius:10px;overflow:hidden;margin-top:16px}.meter i{display:block;background:#7cae86;width:0;height:100%;transition:width .5s}.advanced{margin-top:20px;border:1px solid var(--line);border-radius:8px;background:transparent}summary{cursor:pointer;padding:17px 20px;font-size:13px;font-weight:650}.advanced-content{padding:0 20px 20px}.fields{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:14px}label{display:block;color:var(--muted);font-size:12px}select,input{width:100%;min-height:42px;padding:9px;background:#fff;color:var(--ink);border:1px solid #ccd6cc;border-radius:8px;font:inherit;font-size:13px;margin-top:5px}.advanced-note{font-size:12px;color:var(--muted)}footer{display:flex;justify-content:space-between;gap:14px;margin-top:23px;color:var(--muted);font-size:11px}a{color:inherit}#feedback{min-height:24px;margin-top:12px;font-size:12px;color:var(--green)}#feedback.error{color:var(--red)}
.wifi-fields{grid-template-columns:repeat(2,minmax(0,1fr))}.wifi-status{margin-bottom:18px}.wifi-status strong{overflow-wrap:anywhere}.checkbox{display:flex;align-items:center;gap:8px;margin-top:12px}.checkbox input{width:auto;min-height:0;margin:0}.danger{color:var(--red)}#wifiFeedback{min-height:24px;margin:14px 0 0;font-size:12px;color:var(--muted)}#wifiFeedback.error{color:var(--red)}.forget-confirm{margin-top:18px;padding:16px;border:1px solid var(--line);border-radius:8px}.forget-confirm p{margin:0;font-size:13px}
@media(max-width:640px){main{padding:22px 16px}header{margin-bottom:30px}.layout{grid-template-columns:1fr}.card{padding:21px}.hero{min-height:290px}.freshness{align-items:flex-start;gap:12px;flex-direction:column}.fields{grid-template-columns:1fr 1fr}.time{font-size:58px}.device{font-size:10px}footer{flex-wrap:wrap}h1{font-size:26px}}
@media(max-width:480px){.wifi-fields{grid-template-columns:1fr}}
@media(prefers-reduced-motion:reduce){*{transition:none!important}}
</style>
</head>
<body>
<main>
<header><div class="brand"><span class="mark" aria-hidden="true"><svg width="21" height="21" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.7"><path d="M6 5a10 10 0 0 0 0 14M18 5a10 10 0 0 1 0 14M9 8a6 6 0 0 0 0 8M15 8a6 6 0 0 1 0 8"/><circle cx="12" cy="12" r="1.2" fill="currentColor"/></svg></span>clocksync evolved</div></header>
<h1>Transmitter status</h1>
<div class="layout">
<section class="card hero" aria-label="Transmission status">
<div><div class="status" id="badge" role="status"><span class="dot"></span><span id="state">Connecting…</span></div><div class="time" id="clock">--:--:--</div><div class="date" id="date">Waiting for the device</div><p class="message" id="description">Loading device status.</p></div>
<div class="actions"><button class="primary" id="toggle" disabled>Pause transmission</button><button id="sync" disabled>Sync now</button></div>
</section>
<section class="card" aria-label="Connection and radio signal"><h2 class="section-label">Connection &amp; signal</h2>
<div class="row"><span>Time signal</span><strong id="station">—</strong></div>
<div class="row"><span>Carrier</span><strong id="frequency">—</strong></div>
<div class="row"><span>Wi-Fi</span><strong id="wifi">—</strong></div>
<div class="row"><span>Last sync</span><strong id="lastSync">—</strong></div>
<div class="row"><span>Time sync</span><strong id="ntp">—</strong></div>
<div class="row"><span>Status LED</span><strong id="ledStatus">—</strong></div>
</section>
<section class="card freshness" aria-label="Time validity"><div class="copy"><h2 class="section-label" style="margin-bottom:4px">Time remaining without a new sync</h2><p>Transmission stops automatically after 23 hours without a confirmed time sync.</p><div class="meter" aria-hidden="true"><i id="meter"></i></div></div><div class="remaining" id="remaining">—</div></section>
</div>
<div id="feedback" role="status" aria-live="polite"></div>
<details class="advanced"><summary>Radio settings</summary><div class="advanced-content"><form id="settings">
<div class="fields">
<label>Time signal<select name="station" id="settingStation"><option value="3">DCF77 · Germany</option><option value="0">JJY · 40 kHz</option><option value="1">JJY · 60 kHz</option><option value="2">WWVB · USA</option><option value="5">MSF · United Kingdom</option><option value="4">BSF · Taiwan</option><option value="6">BPC · China</option></select></label>
<label>Antenna pin<select name="pin" id="settingPin"><option>19</option><option>21</option><option>22</option><option>23</option><option>25</option><option>26</option><option>32</option><option>33</option></select></label>
<label>Drive strength<select name="drive" id="settingDrive"><option value="0">0 · Minimum</option><option value="1">1 · Low</option><option value="2">2 · Normal</option><option value="3">3 · Maximum</option></select></label>
<label>Status LED<select name="led" id="settingLed"><option value="1">Show status colours</option><option value="0">Always off</option></select></label>
<label>Daylight saving flag<select name="dst" id="settingDst"><option value="2">Automatic</option><option value="0">Force standard time</option><option value="1">Force daylight saving time</option></select></label>
</div><p class="advanced-note">Default: DCF77 and Central European time. Other signals may require a different time zone in the firmware. Always use a series resistor with the antenna.</p>
<button type="submit" id="apply">Save radio settings</button>
</form></div></details>
<details class="advanced" id="wifiSettings"><summary>Wi-Fi settings</summary><div class="advanced-content">
<div class="wifi-status"><div class="row"><span>Current network</span><strong id="wifiCurrent">—</strong></div><div class="row"><span>Next startup</span><strong id="wifiNext">—</strong></div></div>
<p class="advanced-note">Save a network in advance. Changes take effect only after you restart the device; the current connection stays active. Networks must support 2.4 GHz.</p>
<form id="wifiForm"><div class="fields wifi-fields">
<label for="wifiSSID">Network name (SSID)<input id="wifiSSID" name="ssid" type="text" maxlength="32" required autocomplete="off" autocapitalize="none" spellcheck="false"></label>
<label for="wifiPSK">Password (PSK)<input id="wifiPSK" name="psk" type="password" minlength="8" maxlength="64" required autocomplete="new-password" autocapitalize="none" spellcheck="false"></label>
</div><label class="checkbox"><input id="wifiShowPassword" type="checkbox">Show password</label>
<label class="checkbox"><input id="wifiOpen" type="checkbox">Open network (no password)</label>
<p class="advanced-note">Enter the password again when saving a network. Saved passwords are never displayed. If the new network cannot be reached, Wi-Fi setup becomes available after 10 minutes.</p>
<div class="actions"><button class="primary" id="wifiSave" type="submit" disabled>Save for next startup</button><button class="danger" id="wifiForget" type="button" disabled>Forget Wi-Fi…</button></div></form>
<div class="forget-confirm" id="wifiForgetConfirmation" role="group" aria-label="Confirm forgetting Wi-Fi" hidden><p>Clear the saved network and open Wi-Fi setup at the next startup? The current connection stays active.</p><div class="actions"><button class="danger" id="wifiForgetConfirm" type="button" disabled>Forget Wi-Fi</button><button id="wifiForgetCancel" type="button">Cancel</button></div></div>
<p id="wifiFeedback" role="status" aria-live="polite">Open this section to load Wi-Fi settings.</p>
</div></details>
<footer><span id="connection">Waiting for status</span><span><a href="/status.txt">Technical status</a> · <span id="version">clocksync evolved</span></span></footer>
</main>
<script>
(function(){
'use strict';
var current=null,receivedAt=0,timer=null,busy=false,loading=false,dirty=false,loadedSettings=false,failures=0;
var clockTimer=null,clockSampleMs=NaN;
var wifiToken='',wifiLoading=false,wifiDirty=false,wifiCanEdit=false;
var dateFormat=new Intl.DateTimeFormat('en-GB',{timeZone:'UTC',weekday:'long',day:'numeric',month:'long',year:'numeric'});
var el=function(id){return document.getElementById(id);};
var labels={transmitting:'Transmitting',paused:'Paused',waiting:'Waiting for time',stale:'Stopped – time expired',aligning:'Preparing transmission',error:'Transmitter could not start'};
var descriptions={
transmitting:'Sending the time signal to nearby clocks.',
paused:'Transmission is paused. Time synchronisation continues in the background.',
waiting:'Transmission starts at the next full minute after a confirmed time sync.',
stale:'The last confirmed time sync has expired. Transmission resumes after a new time sync.',
aligning:'Time is valid. Waiting for the next full minute to send a complete time message.',
error:'Transmission is stopped because the radio signal could not start. See technical status.'};
function feedback(text,error){el('feedback').textContent=text;el('feedback').className=error?'error':'';}
function controls(){el('toggle').disabled=busy||!current||failures>0;el('sync').disabled=busy||!current||failures>0;el('apply').disabled=busy||!current||failures>0;wifiControls();}
function countdown(seconds){seconds=Math.max(0,Math.floor(seconds));return Math.floor(seconds/3600)+' h '+String(Math.floor(seconds%3600/60)).padStart(2,'0')+' min';}
function paintClock(){
clearTimeout(clockTimer);
if(!current||document.hidden)return;
var elapsed=(performance.now()-receivedAt)/1000;
var clockMs=clockSampleMs+elapsed*1000;
if(Number.isFinite(clockMs)){
var d=new Date(clockMs);
el('clock').textContent=d.toISOString().slice(11,19);
el('date').textContent=dateFormat.format(d);
}else{el('clock').textContent='--:--:--';el('date').textContent='Local time · waiting for NTP';}
var remaining=Math.max(0,current.remainingSeconds-elapsed);
el('remaining').textContent=current.syncAgeSeconds<0?'Waiting for sync':countdown(remaining);
el('meter').style.width=(remaining/(23*3600)*100)+'%';
if(performance.now()-receivedAt>10000){el('connection').textContent='Status delayed · reconnecting';}
// Follow the device's second boundary, independently of the two-second status poll.
clockTimer=setTimeout(paintClock,Number.isFinite(clockMs)?1000-(clockMs%1000)+5:1000);
}
function show(s){
current=s;receivedAt=performance.now();failures=0;
// Keep the fractional second; dropping it on every poll makes the display stutter.
clockSampleMs=s.localTime?Date.parse(s.localTime.replace(' ','T')+'Z')+(s.localMilliseconds||0):NaN;
el('badge').className='status '+s.state;
el('state').textContent=labels[s.state]||s.state;
el('description').textContent=descriptions[s.state]||'';
if(s.state==='transmitting'&&!s.led)el('description').textContent='Sending the time signal to nearby clocks. The status LED is disabled in settings.';
else if(s.state==='transmitting'&&!s.wifi)el('description').textContent='Transmitting using the last synced time. Wi-Fi is disconnected; transmission stops when the time sync is 23 hours old.';
else if(s.state==='transmitting'&&s.ledState==='yellow')el('description').textContent='Transmitting using the last synced time. A new time sync is taking longer than expected.';
if(s.state==='paused'&&!s.ntp)el('description').textContent='Transmission and automatic time sync are disabled.';
var lightLabels={off:'Off',green:'Green',yellow:'Yellow · waiting for time sync',red:'Red · Wi-Fi disconnected',stopped:'Blinking red · transmission stopped'};
el('ledStatus').textContent=lightLabels[s.ledState]||(!s.led?'Off':'—');
el('toggle').textContent=s.requested?'Pause transmission':'Resume transmission';
el('station').textContent=s.stationName;
var hz=s.frequency||s.targetFrequency;
el('frequency').textContent=(hz/1000).toLocaleString('en-GB',{maximumFractionDigits:1})+' kHz'+(!s.frequency?' · planned':'');
el('wifi').textContent=s.wifi?'Connected':'Disconnected';
el('lastSync').textContent=s.lastSync?s.lastSync.slice(0,16):'No confirmed sync';
el('ntp').textContent=s.syncPending?'Sync requested':s.ntp?'Automatic':'Disabled';
el('version').textContent='v'+s.version;
el('connection').textContent='';
if(!loadedSettings||!dirty){
['Station','Pin','Drive','Dst'].forEach(function(k){el('setting'+k).value=String(s[k.toLowerCase()]);});
el('settingLed').value=s.led?'1':'0';loadedSettings=true;
}
paintClock();controls();
}
function api(url,options){
var controller=new AbortController(),timeout=setTimeout(function(){controller.abort();},4000);
options=options||{};options.signal=controller.signal;options.cache='no-store';
return fetch(url,options).then(function(r){
if(r.ok)return r;
return r.text().then(function(body){var message='Request failed ('+r.status+').';try{var data=JSON.parse(body);if(data.message)message=data.message;}catch(ignore){}throw new Error(message);});
}).finally(function(){clearTimeout(timeout);});
}
function schedule(){clearTimeout(timer);if(!document.hidden)timer=setTimeout(refresh,failures?Math.min(10000,2000*Math.pow(2,failures)):2000);}
function refresh(){
if(loading||busy){schedule();return Promise.resolve();}
loading=true;
return api('/api/status').then(function(r){return r.json();}).then(show).catch(function(){
failures++;el('connection').textContent='Device unreachable · retrying';
el('badge').className='status';el('state').textContent='No connection';
el('description').textContent='Transmission status cannot be confirmed right now. Retrying the connection.';
controls();
}).finally(function(){loading=false;schedule();});
}
function command(c){
if(busy)return;busy=true;controls();feedback('');
return api('/cmd',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'c='+encodeURIComponent(c)})
.then(function(r){return r.text();}).then(function(){feedback(c==='u'?'Time sync requested. A confirmed sync will appear once a reply is received.':'Change received.');})
.catch(function(){feedback('Could not send the change. Check the connection.',true);})
.finally(function(){busy=false;controls();refresh();});
}
el('toggle').addEventListener('click',function(){if(current)command(current.requested?'e0':'e1');});
el('sync').addEventListener('click',function(){command('u');});
el('settings').addEventListener('input',function(){dirty=true;});
el('settings').addEventListener('submit',function(ev){
ev.preventDefault();if(busy)return;busy=true;controls();feedback('');
var form=new URLSearchParams(new FormData(ev.target));
api('/api/settings',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:form.toString()})
.then(function(r){return r.json();}).then(function(s){dirty=false;show(s);feedback('Radio settings saved.');})
.catch(function(){feedback('Could not save settings. Check the connection.',true);})
.finally(function(){busy=false;controls();schedule();});
});
function wifiFeedback(text,error){el('wifiFeedback').textContent=text;el('wifiFeedback').className=error?'error':'';}
function wifiControls(){
var disabled=busy||wifiLoading||!wifiToken||!wifiCanEdit||failures>0;
el('wifiSave').disabled=disabled;el('wifiForget').disabled=disabled;el('wifiForgetConfirm').disabled=disabled;
el('wifiForgetCancel').disabled=busy||wifiLoading;
}
function showWifi(s){
wifiToken=s.token||'';wifiCanEdit=!!s.canEdit;
el('wifiCurrent').textContent=s.currentSSID||'No network';
el('wifiNext').textContent=(s.forceAP?'Setup network: clocksync':s.nextSSID||'No network')+(s.pending?' · pending restart':'');
if(!wifiDirty)el('wifiSSID').value=s.forceAP?'':s.nextSSID||s.currentSSID||'';
wifiFeedback(s.message||'Changes apply after the next restart.',false);wifiControls();
}
function loadWifi(){
if(busy||wifiLoading)return;
wifiLoading=true;wifiControls();wifiFeedback('Loading Wi-Fi settings…',false);
api('/api/wifi/next').then(function(r){return r.json();}).then(showWifi)
.catch(function(error){wifiFeedback(error.name==='AbortError'?'Request timed out. Close and reopen this section to retry.':error.message,true);})
.finally(function(){wifiLoading=false;wifiControls();});
}
function clearWifiPassword(){
el('wifiPSK').value='';el('wifiPSK').type='password';el('wifiPSK').disabled=false;el('wifiPSK').required=true;
el('wifiOpen').checked=false;el('wifiShowPassword').checked=false;
}
function hideForgetConfirmation(){el('wifiForgetConfirmation').hidden=true;el('wifiForget').hidden=false;}
function writeWifi(url,form){
if(busy||wifiLoading||!wifiToken||!wifiCanEdit)return;
busy=true;controls();form.set('token',wifiToken);wifiFeedback('Saving Wi-Fi settings…',false);
api(url,{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:form.toString()})
.then(function(r){return r.json();}).then(function(s){wifiDirty=false;clearWifiPassword();hideForgetConfirmation();showWifi(s);})
.catch(function(error){wifiFeedback(error.name==='AbortError'?'Request timed out. Reopen this section to check whether the change was saved.':error.message,true);})
.finally(function(){busy=false;controls();schedule();});
}
el('wifiSettings').addEventListener('toggle',function(){if(this.open)loadWifi();});
el('wifiForm').addEventListener('input',function(){wifiDirty=true;});
el('wifiShowPassword').addEventListener('change',function(){el('wifiPSK').type=this.checked?'text':'password';});
el('wifiOpen').addEventListener('change',function(){el('wifiPSK').disabled=this.checked;el('wifiPSK').required=!this.checked;});
el('wifiForm').addEventListener('submit',function(ev){
ev.preventDefault();var form=new URLSearchParams(new FormData(ev.target));form.set('psk',el('wifiOpen').checked?'':el('wifiPSK').value);writeWifi('/api/wifi/next',form);
});
el('wifiForget').addEventListener('click',function(){this.hidden=true;el('wifiForgetConfirmation').hidden=false;el('wifiForgetCancel').focus();});
el('wifiForgetCancel').addEventListener('click',hideForgetConfirmation);
el('wifiForgetConfirm').addEventListener('click',function(){writeWifi('/api/wifi/reset',new URLSearchParams());});
document.addEventListener('visibilitychange',function(){clearTimeout(timer);clearTimeout(clockTimer);if(!document.hidden){paintClock();refresh();}});
refresh();
})();
</script>
</body></html>

)CLOCKSYNC";

