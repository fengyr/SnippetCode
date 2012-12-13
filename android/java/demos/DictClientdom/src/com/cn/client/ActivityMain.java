package com.cn.client;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

public class ActivityMain extends Activity {
	private Button start;
	//private ExampleList examplelist;
	private TextView chinese;
	private ListView example;
	private EditText condition;
    private OnClickListener listen;
    private ArrayAdapter<ExampleList> adapter;
    private ArrayList<ExampleList> infoList=new ArrayList<ExampleList>();
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        condition=(EditText)findViewById(R.id.condition);
        start=(Button)findViewById(R.id.start);
        chinese=(TextView)findViewById(R.id.chinese);
        example=(ListView)findViewById(R.id.list);
        
        adapter = new ArrayAdapter<ExampleList>(this,android.R.layout.simple_list_item_1,infoList);
        example.setAdapter(adapter);
        
        listen=new OnClickListener()
        {
        	public void onClick(View v)
        	{
        		
        		String con=condition.getText().toString();
        		
    			try
    			{
    				String str="http://dict.cn/ws.php?q="+con;
    				URL url=new URL(str);
        		    
        		         
        		    URLConnection connection = url.openConnection(); 
        		       
        		    HttpURLConnection httpConnection = (HttpURLConnection)connection; 
        		    int responseCode = httpConnection.getResponseCode(); 

        		    if (responseCode == HttpURLConnection.HTTP_OK) { 
        		      InputStream in = httpConnection.getInputStream(); 
        		          
        		      DocumentBuilderFactory dbfactory = DocumentBuilderFactory.newInstance();
        		      DocumentBuilder db = dbfactory.newDocumentBuilder();

        		     
        		      Document dom = db.parse(in);      
        		      Element docEle = dom.getDocumentElement();
        		        
        		     NodeList nod=docEle.getElementsByTagName("dict");
        		     Element dict=(Element)nod.item(0);
        		     Element def=(Element)dict.getElementsByTagName("def").item(0);
        		     String defstr=def.getFirstChild().getNodeValue();
        		     chinese.setText(defstr);
        		     
        		      infoList.clear();
        		          
        		      
        		      
        		      NodeList nl = docEle.getElementsByTagName("sent");
        		      if (nl != null && nl.getLength() > 0) {
        		        for (int i = 0 ; i < nl.getLength(); i++) {
        		          Element sent = (Element)nl.item(i);
        		          Element orig = (Element)sent.getElementsByTagName("orig").item(0);
        		          Element trans = (Element)sent.getElementsByTagName("trans").item(0);
        		          

        		          String origstr = orig.getFirstChild().getNodeValue();
        		          String  transstr= trans.getFirstChild().getNodeValue();
        		
        		              
        		          ExampleList info = new ExampleList(origstr,transstr);

        		         
        		          newEntry(info);
        		        }
        		      }
        		    }
        		  } catch (Exception e) {
        		    e.printStackTrace();
        		  }
        		
        	}
        };
        
        start.setOnClickListener(listen);
        
        
    }
    private void newEntry(ExampleList info) {
    	  
    	  infoList.add(info);

    	  
    	  adapter.notifyDataSetChanged();
    	}
    
   public class ExampleList{
	   public String orig;
	   public String trans;
	   
	   public ExampleList(String or,String tr)
	   {
		   this.orig=or;
		   this.trans=tr;
	   }
	   
	   public String toString()
	   {
		   return "Àý¾ä\n"+orig+"\n·­Òë\n"+trans;
	   }
	   
    	
    }
}