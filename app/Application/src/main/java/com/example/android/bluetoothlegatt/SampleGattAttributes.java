
package com.example.android.bluetoothlegatt;

import java.util.HashMap;


public class SampleGattAttributes {
    private static HashMap<String, String> attributes = new HashMap();


    static {
        attributes.put("180C", "Greeting");
    }

    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        return name == null ? defaultName : name;
    }
}
