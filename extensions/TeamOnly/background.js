// 加载本地配置
const config_local_url = chrome.runtime.getURL("config/config.json");

fetch(config_local_url)

// 监听来自content-script的消息
chrome.runtime.onMessage.addListener(function(request, sender, sendResponse)
{
			console.log('收到来自content-script的消息：');
			console.log(request, sender, sendResponse);
			sendResponse('本地配置：' + JSON.stringify(config_local));
});
