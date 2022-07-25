// # Copyright © Stuart Millman <stu.millman15@gmail.com>

using System;

namespace Rune
{
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
