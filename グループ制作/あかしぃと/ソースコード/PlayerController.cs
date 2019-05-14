using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : MonoBehaviour
{
    public float speed;

    Rigidbody rigidBody;
    Vector3 move;
    Vector3 startPos;
    bool start;

    void Start()
    {
        rigidBody = GetComponent<Rigidbody>();
        startPos = gameObject.transform.position;
        start = false;

        InputController.SetDefault();
    }

    void Update()
    {

        float control;

        InputController.Update();

        control = InputController.GetAxis(InputController.Axis.Cross_Horizontal);

        if (control != 0)
        {
            move.x = control;
        }
        else
        {
            move.x = Input.GetAxis("Horizontal");
        }

        if (Input.GetKeyDown(KeyCode.R) || Input.GetKeyDown(KeyCode.Joystick1Button0))
        {
            int sceneIndex = SceneManager.GetActiveScene().buildIndex;
            SceneManager.LoadScene(sceneIndex);
        }
    }

    void FixedUpdate()
    {
        rigidBody.transform.position += move * speed;
    }
}