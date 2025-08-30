package com.turbodata

import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.ReadableMap
import com.facebook.react.bridge.Promise
import com.facebook.react.turbomodule.core.interfaces.TurboModule

abstract class NativeTurboDataSpec internal constructor(reactContext: ReactApplicationContext) : ReactContextBaseJavaModule(reactContext) {
    abstract override fun getName(): String

    abstract fun filterObject(
        dataObject: ReadableArray,
        filterCriteria: ReadableMap,
        sortCriteria: ReadableMap?
    ): ReadableArray
}
