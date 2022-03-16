package com.zhengdianfang.jvmtitest

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.zhengdianfang.jvmtitest.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var manager: JVMTIManager
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        manager = JVMTIManager(this.applicationContext)
        manager.start()

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.gcButton.setOnClickListener {
            System.gc()
        }

    }

    override fun onDestroy() {
        super.onDestroy()
        manager.stop()
    }
}