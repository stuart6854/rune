// # Copyright © Stuart Millman <stu.millman15@gmail.com>

namespace Rune
{
    public struct Vector3
    {
        public float x, y, z;

        public static readonly Vector3 Zero = new Vector3(0, 0, 0);

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }

        public static Vector3 operator *(Vector3 v, float s)
        {
            return new Vector3(v.x * s, v.y * s, v.z * s);
        }

        public static Vector3 operator /(Vector3 v, float s)
        {
            return new Vector3(v.x / s, v.y / s, v.z / s);
        }
    }
}