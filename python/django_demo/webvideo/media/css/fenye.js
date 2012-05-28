<!--
/*

*/

function showPages(name) { 	
	this.name = name;
	this.page = 1;         
	this.pageCount = 1;   
	this.argName = 'page';
	this.showTimes = 1;
	this.perPageCount = 10;		
	this.perCount = 10;
}

showPages.prototype.getPage = function(){ 
	var args = location.search;
	var reg = new RegExp('[\?&]?' + this.argName + '=([^&]*)[&$]?', 'gi');
	var chk = args.match(reg);
	this.page = RegExp.$1;
}
showPages.prototype.checkPages = function(){
	if (isNaN(parseInt(this.page))) this.page = 1;
	if (isNaN(parseInt(this.pageCount))) this.pageCount = 1;
	if (this.page < 1) this.page = 1;
	if (this.pageCount < 1) this.pageCount = 1;
	if (this.page > this.pageCount) this.page = this.pageCount;
	this.page = parseInt(this.page);
	this.pageCount = parseInt(this.pageCount);
}
showPages.prototype.createHtml = function(mode){ 
	var strHtml = '', prevPage = this.page - 1, nextPage = this.page + 1;
	if (mode == '' || typeof(mode) == 'undefined') mode = 0;
	switch (mode) {
		case 0 : 
			strHtml += '<span class="count">Pages: ' + this.page + ' / ' + this.pageCount + '</span>';
			strHtml += '<span class="number">';
			if (prevPage < 1) 
			{
				strHtml += '<span title="First Page">&#171;</span>';
				strHtml += '<span title="Prev Page">&#139;</span>';
			} 
			else 
			{
				strHtml += '<span title="First Page"><a href="javascript:' + this.name + '.toPage(1);">&#171;</a></span>';
				strHtml += '<span title="Prev Page"><a href="javascript:' + this.name + '.toPage(' + prevPage + ');">&#139;</a></span>';
			}
			var ss=0;
			for (var i = 1,n =1; i <= this.pageCount; i++,n++) 
			{
				if (i > 0) 
				{
					if( n==10)
					{
						n = 0;
						ss++;
					}

					//alert(k);
					if( ss*this.perCount< i <= (ss+1)*this.perCount)
					{
						if (i == this.page)
						{
							strHtml += '<span title="Page ' + i + '">[' + i + ']</span>';
						}
						else
						{
							strHtml += '<span title="Page ' + i + '"><a href="javascript:' + this.name + '.toPage(' + i + ');">[' + i + ']</a></span>';
						}
					}
				}
			}
			if (nextPage > this.pageCount) 
			{
				strHtml += '<span title="Next Page">&#155;</span>';
				strHtml += '<span title="Last Page">&#187;</span>';
			} 
			else 
			{
				strHtml += '<span title="Next Page"><a href="javascript:' + this.name + '.toPage(' + nextPage + ');">&#155;</a></span>';
				strHtml += '<span title="Last Page"><a href="javascript:' + this.name + '.toPage(' + this.pageCount + ');">&#187;</a></span>';
			}
			strHtml += '</span><br />';
			break;
		case 1 :
			strHtml += '<span class="count">Pages: ' + this.page + ' / ' + this.pageCount + '</span>';
			strHtml += '<span class="number">';
			if (prevPage < 1) {
				strHtml += '<span title="First Page">&#171;</span>';
				strHtml += '<span title="Prev Page">&#139;</span>';
			} else {
				strHtml += '<span title="First Page"><a href="javascript:' + this.name + '.toPage(1);">&#171;</a></span>';
				strHtml += '<span title="Prev Page"><a href="javascript:' + this.name + '.toPage(' + prevPage + ');">&#139;</a></span>';
			}
			if (this.page % 10 ==0) {
				var startPage = this.page - 9;
			} else {
				var startPage = this.page - this.page % 10 + 1;
			}
			if (startPage > 10) strHtml += '<span title="Prev 10 Pages"><a href="javascript:' + this.name + '.toPage(' + (startPage - 1) + ');">...</a></span>';
			for (var i = startPage; i < startPage + 10; i++) {
				if (i > this.pageCount) break;
				if (i == this.page) {
					strHtml += '<span title="Page ' + i + '">[' + i + ']</span>';
				} else {
					strHtml += '<span title="Page ' + i + '"><a href="javascript:' + this.name + '.toPage(' + i + ');">[' + i + ']</a></span>';
				}
			}
			if (this.pageCount >= startPage + 10) strHtml += '<span title="Next 10 Pages"><a href="javascript:' + this.name + '.toPage(' + (startPage + 10) + ');">...</a></span>';
			if (nextPage > this.pageCount) {
				strHtml += '<span title="Next Page">&#155;</span>';
				strHtml += '<span title="Last Page">&#187;</span>';
			} else {
				strHtml += '<span title="Next Page"><a href="javascript:' + this.name + '.toPage(' + nextPage + ');">&#155;</a></span>';
				strHtml += '<span title="Last Page"><a href="javascript:' + this.name + '.toPage(' + this.pageCount + ');">&#187;</a></span>';
			}
			strHtml += '</span><br />';
			break;
		case 2 :
			strHtml += '<span class="count">Pages: ' + this.page + ' / ' + this.pageCount + '</span>';
			strHtml += '<span class="number">';
			if (prevPage < 1) {
				strHtml += '<span title="First Page">&#171;</span>';
				strHtml += '<span title="Prev Page">&#139;</span>';
			} else {
				strHtml += '<span title="First Page"><a href="javascript:' + this.name + '.toPage(1);">&#171;</a></span>';
				strHtml += '<span title="Prev Page"><a href="javascript:' + this.name + '.toPage(' + prevPage + ');">&#139;</a></span>';
			}
			if (this.page != 1) strHtml += '<span title="Page 1"><a href="javascript:' + this.name + '.toPage(1);">[1]</a></span>';
			if (this.page >= 5) strHtml += '<span>...</span>';
			if (this.pageCount > this.page + 2) {
				var endPage = this.page + 2;
			} else {
				var endPage = this.pageCount;
			}
			for (var i = this.page - 2; i <= endPage; i++) {
				if (i > 0) {
					if (i == this.page) {
						strHtml += '<span title="Page ' + i + '">[' + i + ']</span>';
					} else {
						if (i != 1 && i != this.pageCount) {
							strHtml += '<span title="Page ' + i + '"><a href="javascript:' + this.name + '.toPage(' + i + ');">[' + i + ']</a></span>';
						}
					}
				}
			}
			if (this.page + 3 < this.pageCount) strHtml += '<span>...</span>';
			if (this.page != this.pageCount) strHtml += '<span title="Page ' + this.pageCount + '"><a href="javascript:' + this.name + '.toPage(' + this.pageCount + ');">[' + this.pageCount + ']</a></span>';
			if (nextPage > this.pageCount) {
				strHtml += '<span title="Next Page">&#155;</span>';
				strHtml += '<span title="Last Page">&#187;</span>';
			} else {
				strHtml += '<span title="Next Page"><a href="javascript:' + this.name + '.toPage(' + nextPage + ');">&#155;</a></span>';
				strHtml += '<span title="Last Page"><a href="javascript:' + this.name + '.toPage(' + this.pageCount + ');">&#187;</a></span>';
			}
			strHtml += '</span><br />';
			break;
		case 3 :
			strHtml += '<span class="count">Pages: ' + this.page + ' / ' + this.pageCount + '</span>';
			strHtml += '<span class="arrow">';
			if (prevPage < 1) {
				strHtml += '<span title="First Page">9</span>';
				strHtml += '<span title="Prev Page">7</span>';
			} else {
				strHtml += '<span title="First Page"><a href="javascript:' + this.name + '.toPage(1);">9</a></span>';
				strHtml += '<span title="Prev Page"><a href="javascript:' + this.name + '.toPage(' + prevPage + ');">7</a></span>';
			}
			if (nextPage > this.pageCount) {
				strHtml += '<span title="Next Page">8</span>';
				strHtml += '<span title="Last Page">:</span>';
			} else {
				strHtml += '<span title="Next Page"><a href="javascript:' + this.name + '.toPage(' + nextPage + ');">8</a></span>';
				strHtml += '<span title="Last Page"><a href="javascript:' + this.name + '.toPage(' + this.pageCount + ');">:</a></span>';
			}
			strHtml += '</span><br />';
			break;
		case 4 :
			if (this.pageCount < 1) {
				strHtml += '<select name="toPage" disabled>';
				strHtml += '<option value="0">No Pages</option>';
			} else {
				var chkSelect;
				strHtml += '<select name="toPage" onchange="' + this.name + '.toPage(this);">';
				for (var i = 1; i <= this.pageCount; i++) {
					if (this.page == i) chkSelect=' selected="selected"';
					else chkSelect='';
					strHtml += '<option value="' + i + '"' + chkSelect + '>Pages: ' + i + ' / ' + this.pageCount + '</option>';
				}
			}
			strHtml += '</select>';
			break;
		case 5 :
			strHtml += '<span class="input">';
			if (this.pageCount < 1) {
				strHtml += '<input type="text" name="toPage" value="No Pages" class="itext" disabled="disabled">';
				strHtml += '<input type="button" name="go" value="GO" class="ibutton" disabled="disabled"></option>';
			} else {
				strHtml += '<input type="text" value="Input Page:" class="ititle" readonly="readonly">';
				strHtml += '<input type="text" id="pageInput' + this.showTimes + '" value="' + this.page + '" class="itext" title="Input page" onkeypress="return ' + this.name + '.formatInputPage(event);" onfocus="this.select()">';
				strHtml += '<input type="text" value=" / ' + this.pageCount + '" class="icount" readonly="readonly">';
				strHtml += '<input type="button" name="go" value="GO" class="ibutton" onclick="' + this.name + '.toPage(document.getElementById(\'pageInput' + this.showTimes + '\').value);"></option>';
			}
			strHtml += '</span>';
			break;
		default :
			strHtml = 'Javascript showPage Error: not find mode ' + mode;
			break;
	}
	return strHtml;
}
showPages.prototype.createUrl = function (page) {
	if (isNaN(parseInt(page))) page = 1;
	if (page < 1) page = 1;
	if (page > this.pageCount) page = this.pageCount;
	var url = location.protocol + '//' + location.host + location.pathname;
	var args = location.search;
	var reg = new RegExp('([\?&]?)' + this.argName + '=[^&]*[&$]?', 'gi');
	args = args.replace(reg,'$1');
	if (args == '' || args == null) {
		args += '?' + this.argName + '=' + page;
	} else if (args.substr(args.length - 1,1) == '?' || args.substr(args.length - 1,1) == '&') {
		args += this.argName + '=' + page;
	} else {
		args += '&' + this.argName + '=' + page;
	}
	return url + args;
}
showPages.prototype.toPage = function(page){
	var turnTo = 1;
	if (typeof(page) == 'object') {
		turnTo = page.options[page.selectedIndex].value;
	} else {
		turnTo = page;
	}
	self.location.href = this.createUrl(turnTo);
}
showPages.prototype.printHtml = function(mode){
	this.getPage();
	this.checkPages();
	this.showTimes += 1;
	document.write('<div id="pages_' + this.name + '_' + this.showTimes + '" class="pages"></div>');
	document.getElementById('pages_' + this.name + '_' + this.showTimes).innerHTML = this.createHtml(mode);

}
showPages.prototype.formatInputPage = function(e){
	var ie = navigator.appName=="Microsoft Internet Explorer"?true:false;
	if(!ie) var key = e.which;
	else var key = event.keyCode;
	if (key == 8 || key == 46 || (key >= 48 && key <= 57)) return true;
	return false;
}
//-->
