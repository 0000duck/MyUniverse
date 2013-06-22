﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;

namespace MyUniverseControl
{
    //public struct GlbCommand
    //{
    //    int command;
    //    int ctrl1_ret0;
    //    int iParam1;
    //    int iParam2;
    //    double fParam1;
    //    double fParam2;
    //    char[] cParam1;
    //    char[] cParam2;
    //}

    public class MyUniverseControl
    {
        const int GLB_CMD_PLAY = 1;
        const int GLB_CMD_PAUSE = 2;

        const int GLB_CMD_GET_ROTATECLOCKWISE = 3;
        const int GLB_CMD_SET_ROTATECLOCKWISE = 4;

        const int GLB_CMD_GET_ROTATING = 5;
        const int GLB_CMD_SET_ROTATING = 6;

        const int GLB_CMD_GET_POLELATITUDE = 7;
        const int GLB_CMD_SET_POLELATITUDE = 8;

        const int GLB_CMD_GET_POLELONGITUDE = 9;
        const int GLB_CMD_SET_POLELONGITUDE = 10;

        const int GLB_CMD_GET_CHAPTERNAME = 11;
        const int GLB_CMD_SET_CHAPTERNAME = 12;

        const int GLB_CMD_GET_PAGENAME = 13;
        const int GLB_CMD_SET_PAGENAME = 14;

        const int GLB_CMD_GET_ROTATERATE = 15;
        const int GLB_CMD_SET_ROTATERATE = 16;

        public void Play()
        {
            //MessageBox.Show("Play");
            SendCmd(GLB_CMD_PLAY);
        }
        public void Pause()
        {
            //MessageBox.Show("Pause");
            SendCmd(GLB_CMD_PAUSE);
        }
        public void Test()
        {
            //MessageBox.Show("Test");
            SendCmd(3, 2, 3, 4.0, 5.0, "6", "7测试");
            byte[] Buffer = new byte[1056];
            UdpRecv(9161, Buffer);
        }
        public bool GetRotateClockwise()
        {
            SendCmd(GLB_CMD_GET_ROTATECLOCKWISE);

            int command = 0, iParam1 = 0, iParam2 = 0;
            double fParam1 = 0, fParam2 = 0;
            string cParam1 = "", cParam2 = "";
            GetReturn(ref command, ref iParam1, ref iParam2, ref fParam1, ref fParam2, ref cParam1, ref cParam2);
            if (iParam1 == 1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        public void SetRotateClockwise(bool bClockwise)
        {
            int iClockwise;
            if (bClockwise)
            {
                iClockwise = 1;
            }
            else
            {
                iClockwise = 0;
            }
            SendCmd(GLB_CMD_SET_ROTATECLOCKWISE, iClockwise);
        }
        public void SetRotating(bool bRotating)
        {
            int iRotating;
            if (bRotating)
            {
                iRotating = 1;
            }
            else
            {
                iRotating = 0;
            }
            SendCmd(GLB_CMD_SET_ROTATING, iRotating);
        }
        public int GetRotateRate()
        {
            SendCmd(GLB_CMD_GET_ROTATERATE);
            int command = 0, iParam1 = 0, iParam2 = 0;
            double fParam1 = 0, fParam2 = 0;
            string cParam1 = "", cParam2 = "";
            GetReturn(ref command, ref iParam1, ref iParam2, ref fParam1, ref fParam2, ref cParam1, ref cParam2);
            return iParam1;
        }
        public void SetRotateRate(int rate)
        {
            SendCmd(GLB_CMD_SET_ROTATERATE, rate);
        }

        private static void WriteString(IntPtr basePtr, string value, int offset, int length)
        {
            int pos = 0;
            byte[] bytes = Encoding.Default.GetBytes(value);
            while (pos < length)
            {
                if (pos < bytes.Length)
                    Marshal.WriteByte(basePtr, offset, bytes[pos]);
                else
                    Marshal.WriteByte(basePtr, offset, 0);

                pos++;
                offset++;
            }
        }
        private static void ReadString(IntPtr basePtr, ref string value, int offset, int length)
        {
            int pos = 0;
            byte[] bytes = new byte[length];
            while (pos < length - 1)
            {
                if (pos < bytes.Length)
                    bytes[pos] = Marshal.ReadByte(basePtr, offset);
                else
                    bytes[pos] = 0;
                pos++;
                offset++;
            }
            value = Encoding.Default.GetString(bytes);
        }

        public unsafe bool GetReturn(
            ref int command,
            ref int iParam1, ref int iParam2,
            ref double fParam1, ref double fParam2,
            ref string cParam1, ref string cParam2)
        {
            byte[] Buffer = new byte[1056];
            command = 0;
            iParam1 = 0;
            iParam2 = 0;
            fParam1 = 0;
            fParam2 = 0;
            cParam1 = "";
            cParam2 = "";
            int ctrl1_ret0;

            if (!UdpRecv(9161, Buffer))
            {
                return false;
            }
            IntPtr ptr = Marshal.AllocHGlobal(1056);
            Marshal.Copy(Buffer, 0, ptr, 1056);
            command = Marshal.ReadInt32(ptr);
            ctrl1_ret0 = Marshal.ReadInt32(ptr + 4);
            iParam1 = Marshal.ReadInt32(ptr + 8);
            iParam2 = Marshal.ReadInt32(ptr + 12);
            double* pfParam1 = (double*)(ptr + 16); //fParam1
            fParam1 = *pfParam1;
            double* pfParam2 = (double*)(ptr + 24); //fParam2
            fParam2 = *pfParam2;
            ReadString(ptr, ref cParam1, 32, 512);
            ReadString(ptr, ref cParam1, 544, 512);
            Marshal.FreeHGlobal(ptr);
            return true;
        }
        public unsafe void SendCmd(
            int command,
            int iParam1 = 0, int iParam2 = 0,
            double fParam1 = 0, double fParam2 = 0,
            string cParam1="", string cParam2="")
        {
            IntPtr ptr = Marshal.AllocHGlobal(1056);
            Marshal.WriteInt32(ptr, 0, command);    //command
            Marshal.WriteInt32(ptr, 4, 1);          //ctrl1_ret0
            Marshal.WriteInt32(ptr, 8, iParam1);    //iParam1
            Marshal.WriteInt32(ptr, 12, iParam2);   //iParam2
            double* pfParam1 = (double*)(ptr + 16); //fParam1
            *pfParam1 = fParam1;
            double* pfParam2 = (double*)(ptr + 24); //fParam2
            *pfParam2 = fParam2;
            WriteString(ptr, cParam1, 32, 512);     //cParam1
            WriteString(ptr, cParam2, 544, 512);    //cParam2
            
            byte[] buffer = new byte[1056];
            Marshal.Copy(ptr, buffer, 0, 1056);
            UdpSend("127.0.0.1", 9160, buffer, 1056);
            Marshal.FreeHGlobal(ptr);
        }

        public unsafe void UdpSend(string sIP, int iPort, byte[] Buffer, int length)
        {
            //设置服务IP，设置TCP端口号
            IPEndPoint ip = new IPEndPoint(IPAddress.Parse(sIP), iPort);

            //定义网络类型，数据连接类型和网络协议UDP
            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            server.SendTo(Buffer, length, SocketFlags.None, ip);
            server.Close();
        }

        public unsafe bool UdpRecv(int iPort, byte[] Buffer)
        {
            IPEndPoint MyIP = new IPEndPoint(IPAddress.Any, iPort);
            IPEndPoint SenderIP = new IPEndPoint(IPAddress.Any, 0);
            EndPoint Remote = (EndPoint)SenderIP;

            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            server.Bind(MyIP);

            server.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 1000);
            int recv = 0;

            try
            {
                recv = server.ReceiveFrom(Buffer, ref Remote);
            }
            catch (SocketException e)
            {
                if (e.SocketErrorCode == SocketError.TimedOut)
                {
                    //MessageBox.Show("Time out");
                }
                server.Close();
                return false;
            }
            server.Close();
            return true;
        }
    }
}
