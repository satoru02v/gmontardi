/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "It_posix_queue.h"

static UINT32 Testcase(VOID)
{
    INT32 ret;
    CHAR mqname[MQUEUE_STANDARD_NAME_LENGTH] = "";
    CHAR msgrcd[20] = {0};
    struct mq_attr attr = { 0 };
    struct timespec absTimeout = { 0 };
    mqd_t mqueue;

    (void)snprintf_s(mqname, MQUEUE_STANDARD_NAME_LENGTH - 1, MQUEUE_STANDARD_NAME_LENGTH, "/mq028_%d",
        LosCurTaskIDGet());

    attr.mq_msgsize = 20; // 20, queue message size.
    attr.mq_maxmsg = 20; // 20, queue max message size.

    mqueue = mq_open(mqname, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR, &attr);
    ICUNIT_ASSERT_NOT_EQUAL(mqueue, (mqd_t)-1, mqueue);

    ret = mq_timedsend(mqueue, "1234567890123456789", 20, 0, &absTimeout); // 20, mqueue message length.
    ICUNIT_GOTO_EQUAL(ret, MQUEUE_NO_ERROR, ret, EXIT1);

    ret = mq_receive(mqueue, msgrcd, 20, NULL); // 20, mqueue message length.
    ICUNIT_GOTO_EQUAL(ret, 20, ret, EXIT1); // 20, Here, assert the ret.

    ret = mq_close(mqueue);
    ICUNIT_GOTO_EQUAL(ret, MQUEUE_NO_ERROR, ret, EXIT1);

    mqueue = mq_open(mqname, O_RDWR | O_NONBLOCK, S_IRUSR | S_IWUSR, &attr);
    ICUNIT_GOTO_NOT_EQUAL(mqueue, (mqd_t)-1, mqueue, EXIT1);

    ret = mq_receive(mqueue, msgrcd, 20, NULL); // 20, mqueue message length.
    ICUNIT_GOTO_EQUAL(ret, MQUEUE_IS_ERROR, ret, EXIT1);

    ret = mq_close(mqueue);
    ICUNIT_GOTO_EQUAL(ret, MQUEUE_NO_ERROR, ret, EXIT1);

    ret = mq_unlink(mqname);
    ICUNIT_GOTO_EQUAL(ret, MQUEUE_NO_ERROR, ret, EXIT);

    return MQUEUE_NO_ERROR;
EXIT1:
    mq_close(mqueue);
EXIT:
    mq_unlink(mqname);
    return MQUEUE_NO_ERROR;
}

VOID ItPosixQueue028(VOID) // IT_Layer_ModuleORFeature_No
{
    TEST_ADD_CASE("IT_POSIX_QUEUE_028", Testcase, TEST_POSIX, TEST_QUE, TEST_LEVEL0, TEST_FUNCTION);
}
