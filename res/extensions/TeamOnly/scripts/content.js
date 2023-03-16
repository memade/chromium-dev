/*
{
	"fingerprint":{
		
	},
	"proxy":{
		
	},
	"autofill":{
		
	}	
}
*/

let debug_config_data = {
	"fingerprint": {

	},
	"proxy": {

	},
	"autofill": {

	}
};


//!@ 请求本地配置
chrome.runtime.sendMessage("get-global-config", function (response) {
 //let json_string = JSON.stringify(response);
	let json_string = JSON.stringify(debug_config_data);
 //!@ 解析本地配置


 //alert(json_string);
 //console.log('收到来自后台的回复：' + response);
 //alert('收到来自后台的回复：' + response);
});
