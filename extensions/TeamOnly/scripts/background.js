'use strict';

//!@ 本地配置地址
const g_local_config_url = chrome.runtime.getURL("config/config.json");
//!@ 本地配置缓冲
let g_local_config_buffer = {};
//!@ 加载本地配置方法
function f_load_local_config() {
 fetch(g_local_config_url)
  .then((response) => response.json())
  .then((jobj) => {
   g_local_config_buffer = jobj;
  });
}
//!@ 本地配置数据初始化
f_load_local_config();

//!@ 开启监听
chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
 if (message["id"] == 0x1000) {
  sendResponse(g_local_config_buffer);
 }
 else if (message["id"] == 0x1001) {
  chrome.scripting.executeScript(
   {
    target: { tabId: sender.tab.id },
    files: ['scripts/fingerprint.js'],
   });
 }
 else if (message["id"] == 0x1002) {

  chrome.scripting.executeScript(
   {
    target: { tabId: sender.tab.id },
    files: ['scripts/fingerprint.js'],
   });

  let res = { "tab_id": sender.tab.id };
  sendResponse(res);
 }
});

//!@ 自动代理身份验证
//chrome.webRequest.onBeforeSendHeaders.addListener(
// function (details) {
//  for (var i = 0; i < details.requestHeaders.length; ++i) {
//   if (details.requestHeaders[i].name === 'User-Agent') {
//    details.requestHeaders.splice(i, 1);
//    break;
//   }
//  }
//  return { requestHeaders: details.requestHeaders };
// },
// { urls: ["<all_urls>"] },
// ["blocking", "requestHeaders"]
//);



//function getUserColor() { ... }
//function changeBackgroundColor(backgroundColor) {
// document.body.style.backgroundColor = backgroundColor;
//}

//var nav = new NavigationCollector();

//var eventList = ['onBeforeNavigate', 'onCreatedNavigationTarget',
// 'onCommitted', 'onCompleted', 'onDOMContentLoaded',
// 'onErrorOccurred', 'onReferenceFragmentUpdated', 'onTabReplaced',
// 'onHistoryStateUpdated'];

//eventList.forEach(function (e) {
// chrome.webNavigation[e].addListener(function (data) {
//  if (typeof data)
//   console.log(chrome.i18n.getMessage('inHandler'), e, data);
//  else
//   console.error(chrome.i18n.getMessage('inHandlerError'), e);
// });
//});

// Reset the navigation state on startup. We only want to collect data within a
// session.
chrome.runtime.onStartup.addListener(function () {
 //nav.resetDataStorage();

 var dddd = 0;
});



chrome.webNavigation.onDOMContentLoaded.addListener(async ({ tabID, url }) => {
 //if (url !== 'https://example.com/#inject-programmatic') {
 //}
 //if (url == "https://pixelscan.net/") {
 // chrome.scripting.executeScript({
 //  target: { tabId },
 //  files: ["scripts/fingerprint.js"]
 // });
 //}
 //chrome.scripting.executeScript(
 // {
 //  target: { tabId: tabID },
 //  files: ['scripts/fingerprint.js'],
 // });
});

chrome.declarativeNetRequest.onRuleMatchedDebug.addListener((e) => {
 const msg = `Navigation to ${e.request.url} redirected on tab ${e.request.tabId}.`;
 console.log(msg);
});

//chrome.webRequest.onAuthRequired

//chrome.declarativeNetRequest.onAuthRequired.addListener(
// function (details) {
//  var locked = isLocked();
//  var debug = 0;
// },
// { urls: ["<all_urls>"] },
// ["blocking"]
//);

//chrome.webRequest.onAuthRequired.addListener(
//	function (details) {
//		var locked = isLocked();
//		var idstr = details.requestId.toString();
		
//		if (details.isProxy === true && !locked) {
//			console.log('AUTH - ' + details.requestId);
//			//console.log(JSON.stringify(details));
//			if (!(idstr in calls)) {
//				calls[idstr] = 0;
//			}
//			calls[idstr] = calls[idstr] + 1;
//			var retry = parseInt(localStorage["proxy_retry"]) || DEFAULT_RETRY_ATTEMPTS || 5;
//			if (calls[idstr] >= retry) {
//				lock();
//				chrome.notifications.create(NOTIFICATION_ID, {
//					'type': 'basic',
//					'iconUrl': 'icon_locked_128.png',
//					'title': 'Proxy Auto Auth error',
//					'message': 'A lot of Proxy Authentication requests have been detected. There is probably a mistake in your credentials. For your safety, the extension has been temporary locked. To unlock it, click the save button in the options.',
//					'isClickable': true,
//					'priority': 2
//				}, function (id) {
//					//console.log('notification callback');
//				});
//				calls = {};
//				return ({
//					cancel: true
//				});
//			}
//			//var login = localStorage["proxy_login"];
//			//var password = localStorage["proxy_password"];
//			var login = "liyz888-zone-custom-region-us-session-86977717-sessTime-60";
//			var password = "c646674de314d7f2";
//			if (login && password && !locked) {
//				return ({
//					authCredentials: {
//						'username': login,
//						'password': password
//					}
//				});
//			}
//		}
		
//	},
//	{ urls: ["<all_urls>"] },
//	["blocking"]
//);
