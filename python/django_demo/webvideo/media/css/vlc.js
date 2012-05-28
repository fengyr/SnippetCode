<!--

window.history.forward();

var prevState = 0;
var monitorTimerId = 0;
var sliderScrolling = false;
var ignoreSliderChange = false;

function updateVolume(deltaVol)
{
    var vlc = document.getElementById("vlc");
    vlc.audio.volume += deltaVol;
    document.getElementById("volumeTextField").innerHTML = vlc.audio.volume+"%";
};
function formatTime(timeVal)
{
    var timeHour = Math.round(timeVal / 1000);
    var timeSec = timeHour % 60;
    if( timeSec < 10 )
	timeSec = '0'+timeSec;
    timeHour = (timeHour - timeSec)/60;
    var timeMin = timeHour % 60;
    if( timeMin < 10 )
	timeMin = '0'+timeMin;
    timeHour = (timeHour - timeMin)/60;
    if( timeHour > 0 )
	return timeHour+":"+timeMin+":"+timeSec;
    else
	return timeMin+":"+timeSec;
};
function monitor()
{
    var vlc = document.getElementById("vlc");
    var newState = vlc.input.state;
    if( prevState != newState )
    {
	if( newState == 0 )
	{
	    // current media has stopped 
	    onStop();
	}
	else if( newState == 1 )
	{
	    // current media is openning/connecting
	    onOpen();
	}
	else if( newState == 2 )
	{
	    // current media is buffering data
	    onBuffer();
	}
	else if( newState == 3 )
	{
	    // current media is now playing
	    onPlay();
	}
	else if( vlc.input.state == 4 )
	{
	    // current media is now paused
	    onPause();
	}
	prevState = newState;
    }
    else if( newState == 3 )
    {
	// current media is playing
	onPlaying();
    }
    monitorTimerId = setTimeout("monitor()", 1000);
};

/* actions */
function doGo(targetURL)
{
    var vlc = document.getElementById("vlc");
    var options = new Array(":vout-filter=deinterlace", ":deinterlace-mode=linear");
    vlc.playlist.clear();
    //vlc.playlist.add(targetURL, null, options);
    vlc.playlist.add(targetURL);
    vlc.playlist.play();
    if( monitorTimerId == 0 )
    {
	monitor();
    }
};

function doPlayOrPause()
{
    var vlc = document.getElementById("vlc");
    if( vlc.playlist.isPlaying )
    {
	vlc.playlist.togglePause();
    }
    else
    {
	vlc.playlist.play();
	if( monitorTimerId == 0 )
	{
	    monitor();
	}
    }
};
function doStop()
{
    document.getElementById("vlc").playlist.stop();
    if( monitorTimerId != 0 )
    {
	clearTimeout(monitorTimerId);
	monitorTimerId = 0;
    }
    onStop();
};
function doPlaySlower()
{
    var vlc = document.getElementById("vlc");
    vlc.input.rate = vlc.input.rate / 2;
};
function doPlayFaster()
{
    var vlc = document.getElementById("vlc");
    vlc.input.rate = vlc.input.rate * 2;
};

/* events */

function onOpen()
{
};
function onBuffer()
{
};
function onPlay()
{
    onPlaying();
};
var liveFeedText = new Array("Live", "((Live))", "(( Live ))", "((  Live  ))");
var liveFeedRoll = 0;
function onPlaying()
{
};
function onPause()
{
};
function onStop()
{
};

-->


