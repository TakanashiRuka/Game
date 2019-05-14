using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;



public class MiddleResult : MonoBehaviour
{
    public GameObject case1;
    public GameObject case2;

    public AudioClip[] sound;
    private AudioSource audioSource;

    bool Next;

    bool Select;

    bool End;

    Vector3 trigger;

    public void NextStage()
    {
        switch(BackGround.GetStageNum())
        {
            case 1:
                SceneManager.LoadScene("Stage2");
                break;
            case 2:
                SceneManager.LoadScene("Stage3");
                break;
            case 3:
                SceneManager.LoadScene("Stage4");
                break;
            case 4:
                SceneManager.LoadScene("Stage5");
                break;
            case 5:
                SceneManager.LoadScene("Stage6");
                break;
            case 6:
                SceneManager.LoadScene("Stage7");
                break;
            case 7:
                SceneManager.LoadScene("Stage8");
                break;
        }
    }

    public void StageSelect()
    {
        SceneManager.LoadScene("Selecter");
    }

    public void GameEnd()
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
               Application.Quit();
#endif
    }

    private void Start()
    {
        Next = true;

        Select = false;

        End = false;

        InputController.SetDefault();

        audioSource = gameObject.GetComponent<AudioSource>();
    }

    private void Update()
    {
        int control;

        InputController.Update();

        control = InputController.GetAxisDown(InputController.Axis.Cross_Horizontal);


        Debug.Log(BackGround.GetStageNum());
        if (((Input.GetKeyDown(KeyCode.A) || control == -1) && Select) || ((Input.GetKeyDown(KeyCode.D) || control == 1) && !Next && !Select))
        {
            Next = true;

            Select = false;

            End = false;

            case1.transform.position = new Vector3(0.82f, -0.6f, -2.51f);

            case1.SetActive(true);

            case2.SetActive(false);
        }

        else if (((Input.GetKeyDown(KeyCode.D) || control == 1) && !Select && !End) || ((Input.GetKeyDown(KeyCode.A) || control == -1) && End))
        {
            Next = false;

            Select = true;

            End = false;

            case1.SetActive(false);

            case2.SetActive(true);
        }
        else if ((Input.GetKeyDown(KeyCode.D) || control == 1) && Select || ((Input.GetKeyDown(KeyCode.A) || control == -1) && Next))
        {
            Next = false;

            Select = false;

            End = true;

            case1.transform.position = new Vector3(4.34f, -3.2f, -2.51f);

            case1.SetActive(true);

            case2.SetActive(false);


        }

        if(Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
        {
            audioSource.PlayOneShot(sound[1]);

            if (Next)
            {
                NextStage();
            }

            if(Select)
            {
                SceneManager.LoadScene("Selecter");
            }

            if(End)
            {
                #if UNITY_EDITOR
                       UnityEditor.EditorApplication.isPlaying = false;
                #else
                       Application.Quit();
                #endif
            }
        }
    }


}
