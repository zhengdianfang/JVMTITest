package com.zhengdianfang.jvmtitest

import android.content.Context
import android.os.Build
import android.os.Debug
import java.io.File

class JVMTIManager(context: Context) {
    private external fun native_start()
    private external fun native_stop()

    init {
        val applicationInfo = context.packageManager.getApplicationInfo(context.packageName, 0)
        val soFile = File(applicationInfo.nativeLibraryDir, "libjvmtitest.so")
        val destSoFile = File(context.filesDir, "libjvmtitest.so")
        if (destSoFile.exists()) {
            destSoFile.delete()
        }
        soFile.copyTo(destSoFile)
        if (destSoFile.exists() && Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            System.load(destSoFile.absolutePath)
            Debug.attachJvmtiAgent(destSoFile.absolutePath, null, context.classLoader)
        } else {
            throw IllegalArgumentException()
        }
    }
    fun start() {
       native_start()
    }

    fun stop() {
        native_stop()
    }
}