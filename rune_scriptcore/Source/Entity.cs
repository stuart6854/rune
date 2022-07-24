﻿using System;

namespace Rune
{
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float x = 0, float y = 0, float z = 0)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }

    public class Entity
    {
        public float FloatVar { get; set; }

        public Entity()
        {
            Console.WriteLine("Main constructor!");

            InternalCalls.NativeLog("Stuart", 24);

            Vector3 pos = new Vector3(5, 2.5f, 1);
            InternalCalls.NativeLog_Vector3(ref pos, out Vector3 result);
            Console.WriteLine($"{result.x},{result.y},{result.z}");
            Console.WriteLine("{0}", InternalCalls.NativeLog_Vector3Dot(ref pos));
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says: {value}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }
    }
}
