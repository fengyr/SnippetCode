function  mask(obj){ 
	obj.value=obj.value.replace(/[^\d]/g,'') 
		key1=event.keyCode 
		if  (key1==37  ||  key1==39) 
		{  obj.blur(); 
			nextip=parseInt(obj.name.substr(2,1)) 
				nextip=key1==37?nextip-1:nextip+1; 
			nextip=nextip>=5?1:nextip 
				nextip=nextip<=0?4:nextip 
				eval("ip"+nextip+".focus()") 
		}   
	if(obj.value.length>=3)
	{ 
		if(parseInt(obj.value)>=256  ||  parseInt(obj.value)<=0) 
		{ 
			alert(parseInt(obj.value)+"IP地址错误！") 
				obj.value="" 
				obj.focus() 
				return  false; 
		} 
	}
} 
function  mask_c(obj) 
{ 
	clipboardData.setData('text',clipboardData.getData('text').replace(/[^\d]/g,'')) 
}
