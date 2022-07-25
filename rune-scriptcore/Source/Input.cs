namespace Rune
{
    public class Input
    {
        public static bool IsKeyHeld(Keycode key)
        {
            return InternalCalls.Input_IsKeyHeld((int)key);
        }
    }
}