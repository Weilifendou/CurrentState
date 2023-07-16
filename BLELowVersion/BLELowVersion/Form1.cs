using BLELowVersion;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BLELowVersion
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
        }

        // 异步线程
        public static void RunAsync(Action action)
        {
            ((Action)(delegate ()
            {
                action.Invoke();
            })).BeginInvoke(null, null);
        }

        BleCore bleCore = new BleCore();
        /// <summary>
        /// 存储检测到的设备
        /// </summary>
        List<Windows.Devices.Bluetooth.BluetoothLEDevice> DeviceList = new List<Windows.Devices.Bluetooth.BluetoothLEDevice>();

        /// <summary>
        /// 当前蓝牙服务列表
        /// </summary>
        List<Windows.Devices.Bluetooth.GenericAttributeProfile.GattDeviceService> GattDeviceServices = new List<Windows.Devices.Bluetooth.GenericAttributeProfile.GattDeviceService>();

        /// <summary>
        /// 当前蓝牙服务特征列表
        /// </summary>
        List<Windows.Devices.Bluetooth.GenericAttributeProfile.GattCharacteristic> GattCharacteristics = new List<Windows.Devices.Bluetooth.GenericAttributeProfile.GattCharacteristic>();

        private void btnSearch_Click(object sender, EventArgs e)
        {
            listboxBleDevice.Items.Clear();
            bleCore.StartBleDeviceWatcher();
        }

    }
}