#!/usr/bin/env python3
# Copyright (c) 2023 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import subprocess

from builder.common.logger import logger

def _handle_sys_output(line) -> str:
    text = line.rstrip('').rstrip('\n')

    return text

def exec_sys_command(cmd, is_show_output=True, **kwargs) -> tuple[bool, str]:
    # Run this cmd
    process = subprocess.Popen(cmd,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT,
                                encoding='utf-8',
                                **kwargs)

    # Print output
    stdout = []
    if process.stdout is not None:
        for line in process.stdout:
            text = _handle_sys_output(line)
            stdout.append(text)
            if is_show_output:
                logger.debug(' || ' + text)

    stderr = []
    if process.stderr is not None:
        for line in process.stderr:
            text = _handle_sys_output(line)
            logger.error(' !! ' + text)
            if is_show_output:
                stderr.append(text)

    ret_code = process.wait()

    # Check return code
    if ret_code != 0:
        logger.warning('Failed to exec shell cmd: `{}`'.format(' '.join(cmd)))
        return (False, '\n'.join(stderr))
    else:
        return (True, '\n'.join(stdout))
