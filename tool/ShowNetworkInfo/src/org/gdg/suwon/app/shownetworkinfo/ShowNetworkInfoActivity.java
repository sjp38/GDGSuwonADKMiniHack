
package org.gdg.suwon.app.shownetworkinfo;

import android.app.Activity;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.widget.TextView;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;

public class ShowNetworkInfoActivity extends Activity {
    private static final String TAG = "ShowNetworkInfo";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_show_network_info);

        String info = "";
        try {
            Enumeration<NetworkInterface> ifaces = NetworkInterface.getNetworkInterfaces();
            while (ifaces.hasMoreElements()) {
                NetworkInterface iface = ifaces.nextElement();
                if ("lo".equals(iface.getName())) {
                    continue;
                }

                String currentInfo = "";
                Enumeration<InetAddress> addresses = iface.getInetAddresses();
                while (addresses.hasMoreElements()) {
                    InetAddress address = addresses.nextElement();
                    currentInfo += "\n " + address.getHostAddress();
                }
                if (!currentInfo.equals("")) {
                    info += "network interface name : " + iface.getName() + currentInfo + "\n\n";
                }
            }
        } catch (SocketException e) {
            Log.e(TAG, "Fail to get address", e);
        }
        TextView infoTv = (TextView) findViewById(R.id.networkInfoTextView);
        infoTv.setText(info);
    }
}
