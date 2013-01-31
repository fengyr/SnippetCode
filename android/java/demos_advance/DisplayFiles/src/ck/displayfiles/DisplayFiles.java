package ck.displayfiles;

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class DisplayFiles<bRootDirectory> extends Activity {
	private Button bPhoneDirectory;
	private Button bDataDirectory;
	private Button bDownloadCacheDirectory;
	private Button bExternalStorageDirectory;
	private Button bRootDirectory;
	
	private File fPhoneDirectory;
	private File fDataDirectory;
	private File fDownloadCacheDirectory;
	private File fExternalStorageDirectory;
	private File fRootDirectory;
	
	private String name;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        bPhoneDirectory = (Button)findViewById(R.id.bPhoneDirectory);
        bRootDirectory = (Button)findViewById(R.id.bDataDirectory);
        bDownloadCacheDirectory = (Button)findViewById(R.id.bDownloadCacheDirectory);
        bExternalStorageDirectory = (Button)findViewById(R.id.bExternalStorageDirectory);
        bRootDirectory = (Button)findViewById(R.id.bRootDirectory);
        
        fPhoneDirectory = this.getFilesDir();
        fDataDirectory = Environment.getDataDirectory();
        fDownloadCacheDirectory = Environment.getDownloadCacheDirectory();
        fExternalStorageDirectory = Environment.getExternalStorageDirectory();
        fRootDirectory = Environment.getRootDirectory();
        
        /*
         * 没有存储卡的时候按键无效
         */
        if(Environment.getExternalStorageState().equals(Environment.MEDIA_REMOVED)){
        	bExternalStorageDirectory.setEnabled(false);
        }
        
        /*
         * 访问应用程序手机内存
         */
        bPhoneDirectory.setOnClickListener(new Button.OnClickListener(){
        	public void onClick(View v){
        		String path = fPhoneDirectory.getPath();

        		name=path+"\n";
        		ListFiles(path, "");
        		
        		Toast.makeText(DisplayFiles.this, name, Toast.LENGTH_LONG).show();        		
        	}
        });
      
        /*
         * 访问存储卡
         */
        bExternalStorageDirectory.setOnClickListener(new Button.OnClickListener(){
        	public void onClick(View v){
        		String path = fExternalStorageDirectory.getPath();
        		name=path+"\n";
        		ListFiles(path, "");
        		        		
        		Toast.makeText(DisplayFiles.this, name, Toast.LENGTH_LONG).show();
        	}
        });
        
        /*   
         * 其他无法访问
         * 访问出错
         *     
        bDownloadCacheDirectory.setOnClickListener(new Button.OnClickListener(){
        	public void onClick(View v){
        		String path = fDownloadCacheDirectory.getAbsolutePath();
        		Toast.makeText(DisplayFiles.this, path, Toast.LENGTH_LONG).show();
        	}
        });
        
        bRootDirectory.setOnClickListener(new Button.OnClickListener(){
        	public void onClick(View v){
        		String path = fRootDirectory.getAbsolutePath();
        		Toast.makeText(DisplayFiles.this, path, Toast.LENGTH_LONG).show();
        	}
        });
        
        bDataDirectory.setOnClickListener(new Button.OnClickListener(){
        	public void onClick(View v){
        		String path = fDataDirectory.getAbsolutePath();
        		Toast.makeText(DisplayFiles.this, path, Toast.LENGTH_LONG).show();
        	}
        });
        
        */
    }
   
    private boolean ListFiles(String path, String arr){
    	File file = new File(path); 
    	arr = arr+"\t";                    //arr为层次变量，使目录有层次感
    	
		for(File f:file.listFiles()){
			path = f.getAbsolutePath();
			name = name+arr+f.getName()+"\n";
			
			if(f.isDirectory()==true){
				ListFiles(path, arr);	    //当f为文件夹的时候，进入文件夹中			
			}
		}
		
		return true;
    }
}