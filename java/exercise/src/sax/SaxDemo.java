package sax;

import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.SAXException;
import org.xml.sax.Attributes;

import javax.xml.parsers.SAXParserFactory;
import javax.xml.parsers.SAXParser;

import java.util.Stack;
import java.util.ArrayList;

class Book {
    String author;
    String title;
    String line;

    public void setAuthor(String au) {
        author = au;
    }

    public void setTitle(String ti) {
        title = ti;
    }

    public void setLine(String li) {
        line = li;
    }

    public void show() {
        System.out.println("info: Author" + author + " title " + title + " line " + line);
    }
}

public class SaxDemo extends DefaultHandler {
    
    ArrayList<Book> bookList = new ArrayList<Book>();
    Book book = null;
    Stack tags;

    @Override
    public void startDocument() throws SAXException {
        //System.out.println("startDocument");        

        tags = new Stack();
    }

    @Override
    public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
        //System.out.println("startElement: uri " + uri + " localName " + localName + " qName " + qName);

        tags.push(qName);

        if ( qName.equals("Book") ) {
            if ( book == null ) {
                book = new Book();
            }
        }
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        //System.out.println("endElement: uri " + uri + " localName " + localName + " qName " + qName);

        tags.pop();

        if ( qName.equals("Book") ) {
            if ( book != null ) {
                bookList.add(book);
            }
            book = null;
        }
    }

    @Override
    public void characters(char[] ch, int start, int length) throws SAXException {
        String str = new String(ch, start, length );
        //System.out.println("characters: " + str);

        String tag = (String)tags.peek();
        
        if ( tag.equals("Author") && (book != null) ) {
            book.setAuthor(str);
        }
        else if ( tag.equals("Title") && (book != null) ) {
            book.setTitle(str);
        }
        else if ( tag.equals("Line") && (book != null) ) {
            book.setLine(str);
        }

    }

    @Override
    public void endDocument() throws SAXException {
        //System.out.println("endDocument");

        for (Book bk : bookList) {
            bk.show();        
        }
    }
}
