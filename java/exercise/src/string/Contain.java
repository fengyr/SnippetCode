package string;

public class Contain {
    String mStr;

    public Contain() {
        mStr = new String("，。！？《》（）");
    }

    public boolean containIt(String element) {
        if ( mStr.indexOf(element) >= 0 ) {
            return true;
        }

        return false;
    }
}
