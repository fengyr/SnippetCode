package org.example.test;

import android.app.Activity;

import android.os.Bundle;

import android.content.Context;

import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.graphics.drawable.AnimationDrawable;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;

import android.widget.ImageView;
import android.widget.Button;

public class MyTest extends Activity implements OnClickListener {

    private Animation mAnim;
    private ImageView mImageView;
    private AnimationDrawable mDrawable;
    private Button mButton;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        //loadAnimation(this, R.anim.alpha);
        //loadAnimation(this, R.anim.scale);
        //loadAnimation(this, R.anim.translate);
        //loadAnimation(this, R.anim.rotate);

        loadFrameAnimation();

        mButton = (Button) findViewById(R.id.button);
        mButton.setOnClickListener(this);
    }

    private void loadAnimation(Context context, int id) {
        ViewGroup view = (ViewGroup) findViewById(R.id.container);
        mAnim = AnimationUtils.loadAnimation(context, id);
        view.startAnimation(mAnim);
    }

    private void loadFrameAnimation() {
        mImageView = (ImageView) findViewById(R.id.image);
        mImageView.setBackgroundResource(R.anim.animation);
        mDrawable = (AnimationDrawable) mImageView.getBackground();
    }

    public void onClick(View view) {
        switch ( view.getId() ) {
        case R.id.button :
            mDrawable.stop();
            mDrawable.start();
            break;
        default :
            break;
        }
    }
}
