package io.github.xingray.vulkandemo

import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.Gravity
import android.widget.Button
import android.widget.FrameLayout
import androidx.core.view.ViewCompat
import com.google.androidgamesdk.GameActivity


class MainActivity:GameActivity() {

    companion object{
        init {
            System.loadLibrary("vulkan_demo")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val handler = Handler(Looper.getMainLooper())
        handler.post {
            println("in post callback")
        }
    }

    override fun onCreateSurfaceView() {
        this.mSurfaceView = InputEnabledSurfaceView(this)
        val frameLayout = FrameLayout(this)
        frameLayout.setPadding(20,20,20,20)
        frameLayout.setBackgroundColor(Color.GRAY)
        this.contentViewId = ViewCompat.generateViewId()
        frameLayout.id = contentViewId

        // 添加 SurfaceView
        frameLayout.addView(this.mSurfaceView)

        // 添加按钮
        val button = Button(this)
        button.text = "Change Color"
        button.layoutParams = FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT
        ).apply {
            gravity = Gravity.BOTTOM or Gravity.CENTER_HORIZONTAL
            bottomMargin = 50
        }
        frameLayout.addView(button)

        // 设置点击事件
        button.setOnClickListener {
            changeTriangleColor(gameActivityNativeHandle, randomColor())
        }

        frameLayout.setPadding(100, 200, 100, 200)
        this.setContentView(frameLayout)
        frameLayout.requestFocus()
        mSurfaceView.holder.addCallback(this)
        ViewCompat.setOnApplyWindowInsetsListener(this.mSurfaceView, this)
    }

    fun randomColor(): Int {
        // 定义可选的颜色数组
        val colors = arrayOf(
            Color.BLACK,
            Color.DKGRAY,
            Color.GRAY,
            Color.LTGRAY,
            Color.WHITE,
            Color.RED,
            Color.GREEN,
            Color.BLUE,
            Color.YELLOW,
            Color.CYAN,
            Color.MAGENTA
        )
        // 从数组中随机选择一个颜色并返回
        return colors.random()
    }


    private external fun changeTriangleColor(nativeHandle:Long, color: Int)
}