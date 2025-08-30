package com.turbodata

import com.facebook.proguard.annotations.DoNotStrip
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.ReadableMap
import com.facebook.react.turbomodule.core.interfaces.TurboModule

abstract class NativeTurboDataSpec(reactContext: ReactApplicationContext) : ReactContextBaseJavaModule(reactContext), TurboModule {
    @DoNotStrip
    abstract fun filterObject(dataObject: ReadableArray, filterCriteria: ReadableMap, sortCriteria: ReadableMap?): ReadableArray
}
