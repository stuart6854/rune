using System.Runtime.CompilerServices;

namespace Rune
{
    public static class InternalCalls
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void NativeLog(string text, int parameter);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void NativeLog_Vector3(ref Vector3 param, out Vector3 outResult);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float NativeLog_Vector3Dot(ref Vector3 param);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_GetTranslation(ulong id, out Vector3 param);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetTranslation(ulong id, ref Vector3 param);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Input_IsKeyHeld(int key);
    }
}