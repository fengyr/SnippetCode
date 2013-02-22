package string;

public class Contain {
	static public String TAG = "ContainTag";
	public String mStr;

    public Contain() {
        mStr = new String("，。！？《》（）");
    }

    public boolean containIt(String element) {
        if ( mStr.indexOf(element) >= 0 ) {
            return true;
        }

        return false;
    }
    
    static public String printTag() {
        System.out.println(TAG);
        
        return "printTag invoke success";
    }
}
