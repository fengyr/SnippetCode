package cn.sl.event;

public class Constant {
	public static final String LOGCAT = "logcat";

	public static String getActionTAG(int action) {
		switch (action) {
		case 0:
			return "ACTION_DOWN";
		case 1:
			return "ACTION_UP";
		case 2:
			return "ACTION_MOVE";
		default:
			return "NULL";
		}
	}
}
