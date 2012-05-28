<!--
function line(obj)
{
obj.style.textDecoration='underline';
}
function delline(obj)
{
obj.style.textDecoration='none';
}
function showtree(str)
{
var eval1="span_"+str+".innerHTML=span_"+str+".innerHTML=='+'?'-':'+'";
var eval2=str+"_value.style.display="+str+"_value.style.display=='none'?'':'none'";
eval(eval1);
eval(eval2);
}

function linkTo(url)
{
	if ( event.clientX < 0 || event.clientY < 0 || event.altKey)
	{
		location.href(url)
	}
};

//-->

