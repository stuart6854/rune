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
    }
}