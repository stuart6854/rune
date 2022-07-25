// # Copyright © Stuart Millman <stu.millman15@gmail.com>

using System;

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

        public static Vector3 operator+(Vector3 v, float s)
        {
            return new Vector3(v.x + s, v.y + s, v.z + s);
        }

        public static Vector3 operator*(Vector3 v, float s)
        {
            return new Vector3(v.x * s, v.y * s, v.z * s);
        }
    }

    public class Entity
    {
        public readonly ulong Id;

        public Entity()
        {
            Id = 0;
        }

        internal Entity(ulong id)
        {
            Id = id;
            Console.WriteLine($"Entity ID: {id}");
        }

        public Vector3 Translation
        {
            get
            {
                InternalCalls.Entity_GetTranslation(Id, out Vector3 translation);
                return translation;
            }
            set => InternalCalls.Entity_SetTranslation(Id, ref value);
        }
    }
}
