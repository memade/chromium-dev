chrome.runtime.sendMessage({greeting: '你好，我是content-script呀，我主动发消息给后台！'}, function(response) {
    //console.log('收到来自后台的回复：' + response);
				alert('收到来自后台的回复：' + response);
});



//alert("Hello World , I come from 'content.js!'");