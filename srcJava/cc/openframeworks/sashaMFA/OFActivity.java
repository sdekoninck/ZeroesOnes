package cc.openframeworks.sashaMFA;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.app.ActionBar;
import android.content.Intent;
import cc.openframeworks.OFAndroid;


public class OFActivity extends cc.openframeworks.OFActivity{

	@Override
    public void onCreate(Bundle savedInstanceState)
    { 
        super.onCreate(savedInstanceState);
        String packageName = getPackageName();

        ofApp = new OFAndroid(packageName,this);
    }
	
	@Override
	public void onDetachedFromWindow() {
	}
	
	@Override
	public void onBackPressed() {
	    // nothing to do here
	    // … really      
	}
	
    @Override
    protected void onPause() {
    	System.exit(0);
        super.onPause();
        ofApp.pause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        ofApp.resume();
        
	    View decorView = getWindow().getDecorView();
		// Hide the status bar.
		int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
		decorView.setSystemUiVisibility(uiOptions);
		// Remember that you should never show the action bar if the
		// status bar is hidden, so hide that too if necessary.
		ActionBar actionBar = getActionBar();
		actionBar.hide();

    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    // Override the volume buttons to do nothing
	if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN || keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
		return true;
	}
	else {
		if (OFAndroid.keyUp(keyCode, event)) {
	    return true;
		} else {
		    return super.onKeyUp(keyCode, event);
		}
	}
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
	// Override the volume buttons to do nothing
    if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN || keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
		return true;
	}
	else {
		if (OFAndroid.keyUp(keyCode, event)) {
	    return true;
		} else {
		    return super.onKeyUp(keyCode, event);
		}
	}
    }
    
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
      super.onWindowFocusChanged(hasFocus);
      if(!hasFocus) {
          // Close every kind of system dialog
        Intent closeDialog = new Intent(Intent.ACTION_CLOSE_SYSTEM_DIALOGS);
        sendBroadcast(closeDialog);
      }
    }


	OFAndroid ofApp;
    
	
	
    // Menus
    // http://developer.android.com/guide/topics/ui/menus.html
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	// Create settings menu options from here, one by one or infalting an xml
        return super.onCreateOptionsMenu(menu);
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	// This passes the menu option string to OF
    	// you can add additional behavior from java modifying this method
    	// but keep the call to OFAndroid so OF is notified of menu events
    	if(OFAndroid.menuItemSelected(item.getItemId())){
    		
    		return true;
    	}
    	return super.onOptionsItemSelected(item);
    }
    

    @Override
    public boolean onPrepareOptionsMenu (Menu menu){
    	// This method is called every time the menu is opened
    	//  you can add or remove menu options from here
    	return  super.onPrepareOptionsMenu(menu);
    }
	
}



