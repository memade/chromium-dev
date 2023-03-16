'use strict';

let debug_config_data = {
 "fingerprint": {

 },
 "proxy": {

 },
 "autofill": {

 }
};

//var script = document.createElement("script");
//script.src = chrome.runtime.getURL("scripts/fingerprint.js");
//script.onload = function () {
//	this.remove();
//};
//(document.head || document.documentElement).appendChild(script);

//const { options } = await chrome.storage.local.get('options');
//chrome.scripting.executeScript({
//	target: { tabId },
//	files: ['content-script.js'],
//	...options
//});

//chrome.scripting.executeScript(
//	{
//		target: { tabId: getCurrentTab().id },
//		files: ['script/fingerprint.js'],
//	});



//chrome.scripting.executeScript({
//	target: { tabId: tab.id },
//	files: ['content-script.js']
//});

//chrome.scripting.executeScript({
//	target: { tabId: getCurrentTab() },
//		func: changeBackgroundColor,
//		args: [getUserColor()],
//	})
//	.then(() => console.log("injected a function"));

//var i = document.createElement("script");
//i.text = "(".concat(n, ")('").concat(e, "')"),
//	document.documentElement.appendChild(i).parentNode.removeChild(i),
//	window.removeEventListener(t.g, o),
//	window.addEventListener(t.g, o);
//var a = document.createElement("script");
//a.src = chrome.runtime.getURL("script/fingerprint.js"),
//	a.onload = function (t) {
//		return window.dispatchEvent(new CustomEvent("fpsld", {
//			detail: e
//		}))
//	},
//	document.documentElement.appendChild(a).parentNode.removeChild(a)

/* async function getCurrentTab() {
	let queryOptions = { active: true, lastFocusedWindow: true };
	// `tab` will either be a `tabs.Tab` instance or `undefined`.
	let [tab] = await chrome.tabs.query(queryOptions);
	return tab;
}
const tab = await getCurrentTab(); */

//!@ 请求本地配置
chrome.runtime.sendMessage({
 "id": 0x1000,
 "data_s": ""
}, function (response) {
 //let json_string = JSON.stringify(response);
 let json_string = JSON.stringify(debug_config_data);
 //!@ 解析本地配置


 //alert(json_string);
 //console.log('收到来自后台的回复：' + response);
 //alert('收到来自后台的回复：' + response);
});

//!@ 请求脚本资源
chrome.runtime.sendMessage({
 "id": 0x1001,
 "tab_id":0,
 "script": ""
}, function (response) {

 //chrome.tabs.getCurrent(
 // callback ?: function,
 //)


 //chrome.tabs.executeScript({
 // target: { tabId: tag.id },
 // func: changeBackgroundColor,
 // args: [getUserColor()],
 //})
 // .then(() => console.log("injected a function"));

 //chrome.scripting.executeScript({
 // target: { tabId },
 // files: ["scripts/fingerprint.js"]
 //})
 // .then(() => console.log("injected a function"));

 //let json_string = JSON.stringify(response);
 //let json_string = JSON.stringify(debug_config_data);
 //!@ 解析本地配置
 //alert(json_string);
 //console.log('收到来自后台的回复：' + response);
 //alert(getCurrentTab().tabs.i);
});