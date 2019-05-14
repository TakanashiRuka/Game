using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StageSelect : MonoBehaviour
{
    bool S1;

    bool S2;

    public void Stage1()
    {
        SceneManager.LoadScene("Stage1");
    }


    public void Stage2()
    {
        SceneManager.LoadScene("Stage2");
    }

    private void Start()
    {
        S1 = false;

        S2 = false;
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            S1 = true;

            S2 = false;
        }

        if (Input.GetKeyDown(KeyCode.D))
        {
            S2 = true;

            S1 = false;
        }

        if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
        {
            if (S1)
            {
                SceneManager.LoadScene("Stage1");
            }

            if (S2)
            {
                SceneManager.LoadScene("Stage2");
            }
        }
    }

}
