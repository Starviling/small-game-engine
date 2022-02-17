var eventId = internString("script_event");
var scriptBool = internString("script_bool");

function scriptEvent(event)
{
	event.addBool(scriptBool, true);

	return "Script Success 2";
}

function scriptEventId()
{
	return eventId;
}

function handleEvent(event)
{
	if (event.getBool(scriptBool) == true) {
		printString("The events being generated currently are true.");
	} else {
		printString("Script is now generating false events.");
	}
	return "Script Success 3";
}