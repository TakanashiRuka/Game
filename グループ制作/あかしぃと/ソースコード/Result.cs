using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class Result : MonoBehaviour {

    public GameObject case1;
    public GameObject case2;

    public AudioClip[] sound;
    private AudioSource audioSource;

    bool Select;

    bool End;

    Vector3 trigger;

    public void Stage()
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
        Select = true;

        End = false;

        audioSource = gameObject.GetComponent<AudioSource>();
    }

    private void Update()
    {

        trigger.x = Input.GetAxis("Cont");

        if (Input.GetKeyDown(KeyCode.A) || trigger.x < 0)
        {
            End = false;

            Select = true;

            case1.SetActive(true);

            case2.SetActive(false);
        }

        if (Input.GetKeyDown(KeyCode.D) || trigger.x > 0)
        {
            End = true;

            Select = false;

            case1.SetActive(false);

            case2.SetActive(true);

        }

        if(Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
        {
            audioSource.PlayOneShot(sound[1]);

            if (Select)
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
