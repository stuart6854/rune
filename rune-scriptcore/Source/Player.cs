using System;

using Rune;

namespace Sandbox
{
    public class Player : Entity
    {
        private void OnCreate()
        {
            Console.WriteLine("Player.OnCreate()");
        }

        private void OnUpdate(float deltaTime)
        {
            const float speed = 5.0f;
            var velocity = new Vector3();
            if (Input.IsKeyHeld(Keycode.Keypad8))
                velocity.z += 1.0f;
            if (Input.IsKeyHeld(Keycode.Keypad2))
                velocity.z += -1.0f;
            if (Input.IsKeyHeld(Keycode.Keypad6))
                velocity.x += 1.0f;
            if (Input.IsKeyHeld(Keycode.Keypad4))
                velocity.x += -1.0f;

            velocity *= speed * deltaTime;

            Translation += velocity;
        }
    }
}