using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PageManager : MonoBehaviour {

    float PageTrig;
    bool ClosePage;
    private Animator animator;

    public AudioClip[] sound;
    private AudioSource audioSource;

    // Use this for initialization
    void Start()
    {
        animator = GetComponent<Animator>();
        PageTrig = 0;
        ClosePage = false ;
        animator.SetFloat("Page_Manager", PageTrig);
        animator.SetBool("Back_Select", ClosePage);

        InputController.SetDefault();

        audioSource = gameObject.GetComponent<AudioSource>();
    }
 
	
	// Update is called once per frame
	void Update ()
    {

        int control;

        InputController.Update();

        control = InputController.GetAxisDown(InputController.Axis.Cross_Horizontal);

        switch ((int)PageTrig)
        {
            case 0:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    PageTrig += 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take1->2");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                   // PageTrig -= 1;
                   // animator.SetFloat("Page_Manager", PageTrig);
                }
                
                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    PageTrig += 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }

                if (Input.GetKeyDown(KeyCode.LeftShift))
                {
                    ClosePage = true;
                    PageTrig =  10 ;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage1");
                }
                break;

            case 1:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    PageTrig += 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take1->2");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    PageTrig -= 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }
                /*if (Input.GetKeyDown(KeyCode.S))
                {
                    PageTrig -= 2;
                    animator.SetTrigger("take1->2");
                }*/
                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    PageTrig += 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take1->2");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.LeftShift))
                {
                    ClosePage = true;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage2");
                }
                break;

            case 2:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    PageTrig += 2;
                    //animator.ResetTrigger("take1->2");
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take2->3");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    PageTrig -= 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take2->2-B");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.S))
                {
                    PageTrig -= 2;
                    //animator.ResetTrigger("take1->2");
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take2->2-B");

                }
                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    PageTrig += 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }

                if (Input.GetKeyDown(KeyCode.Return))
                {
                    ClosePage = true;
                    PageTrig = 10;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage3");
                }
                break;

            case 3:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    PageTrig += 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take2->3");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    PageTrig -= 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }
                if (Input.GetKeyDown(KeyCode.S))
                {
                    PageTrig -= 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take2->2-B");
                }
                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    PageTrig += 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take2->3");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.LeftShift))
                {
                    ClosePage = true;
                    PageTrig = 10;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage4");
                }
                break;

            case 4:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    PageTrig += 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take3->4");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    PageTrig -= 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take3->3-B");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.S))
                {
                    PageTrig -= 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take3->3-B");
                }
                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    PageTrig += 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }
                if (Input.GetKeyDown(KeyCode.LeftShift))
                {
                    ClosePage = true;
                    PageTrig = 10;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage5");
                }
                break;

            case 5:
                if (Input.GetKeyDown(KeyCode.W))
                {
                    PageTrig += 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take3->4");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    PageTrig -= 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }
                if (Input.GetKeyDown(KeyCode.S))
                {
                    PageTrig -= 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take3->3-B");
                }
                if (Input.GetKeyDown(KeyCode.D) || control == 1)
                {
                    PageTrig += 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take3->4");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.LeftShift))
                {
                    ClosePage = true;
                    PageTrig = 10;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage6");
                }
                break;

            case 6:
                
                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    PageTrig -= 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take4->4-B");

                    audioSource.PlayOneShot(sound[1]);
                }
                if (Input.GetKeyDown(KeyCode.S))
                {
                    PageTrig -= 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take4->4-B");
                }
                if ((Input.GetKeyDown(KeyCode.D) || control == 1) && (PageTrig < 7))
                {
                    PageTrig += 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }
                if (Input.GetKeyDown(KeyCode.LeftShift))
                {
                    ClosePage = true;
                    PageTrig = 10;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage7");
                }
                break;

            case 7:

                if (Input.GetKeyDown(KeyCode.A) || control == -1)
                {
                    PageTrig -= 1;
                    animator.SetFloat("Page_Manager", PageTrig);
                }
                if (Input.GetKeyDown(KeyCode.S))
                {
                    PageTrig -= 2;
                    animator.SetFloat("Page_Manager", PageTrig);
                    animator.SetTrigger("take4->4-B");
                }
                if (Input.GetKeyDown(KeyCode.LeftShift))
                {
                    ClosePage = true;
                    PageTrig = 10;
                    animator.SetBool("Back_Select", ClosePage);
                }
                if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
                {
                    audioSource.PlayOneShot(sound[2]);

                    SceneManager.LoadScene("Stage8");
                }
                break;

            default:
               // animator.GetCurrentAnimatorStateInfo(0).IsName("Page_Manager");
                if (Input.GetKeyDown(KeyCode.RightShift) &&(ClosePage == true))
                {
                    PageTrig = 0;
                    animator.SetFloat("Page_Manager", PageTrig);
                    ClosePage = false ;
                    animator.SetBool("Back_Select", ClosePage);
                }
                break;
        }
    }
}
